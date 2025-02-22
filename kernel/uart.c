//
// low-level driver routines for 16550a UART.
//

#include <kernel/console.h>
#include <kernel/io.h>
#include <kernel/memlayout.h>
#include <kernel/proc.h>
#include <kernel/spinlock.h>
#include <kernel/types.h>

// the UART control registers are memory-mapped
// at address UART0. this macro returns the
// address of one of the registers.

// the UART control registers.
// some have different meanings for
// read vs write.
// see http://byterunner.com/16550.html
#define RHR 0 // receive holding register (for input bytes)
#define THR 0 // transmit holding register (for output bytes)
#define IER 1 // interrupt enable register
#define IER_RX_ENABLE (1 << 0)
#define IER_TX_ENABLE (1 << 1)
#define FCR 2 // FIFO control register
#define FCR_FIFO_ENABLE (1 << 0)
#define FCR_FIFO_CLEAR (3 << 1) // clear the content of the two FIFOs
#define ISR 2                   // interrupt status register
#define LCR 3                   // line control register
#define LCR_EIGHT_BITS (3 << 0)
#define LCR_BAUD_LATCH (1 << 7) // special mode to set baud rate
#define LSR 5                   // line status register
#define LSR_RX_READY (1 << 0)   // input is waiting to be read from RHR
#define LSR_TX_IDLE (1 << 5)    // THR can accept another character to send

// the transmit output buffer.
struct spinlock uart_tx_lock;
#define UART_TX_BUF_SIZE 32
char uart_tx_buf[UART_TX_BUF_SIZE];
uint64 uart_tx_w; // write next to uart_tx_buf[uart_tx_w % UART_TX_BUF_SIZE]
uint64 uart_tx_r; // read next from uart_tx_buf[uart_tx_r % UART_TX_BUF_SIZE]

extern volatile int panicked; // from printf.c

void uartstart();

void uartinit(void) {
    // disable interrupts.
    WriteRegShift(UART0, UART0_REGSHIFT, IER, 0x00);

    // Set DLAB to 1 to set baud rate.
    WriteRegShift(UART0, UART0_REGSHIFT, LCR, LCR_BAUD_LATCH);

    // LSB for baud rate of 38.4K.
    WriteRegShift(UART0, UART0_REGSHIFT, 0, 0x03);

    // MSB for baud rate of 38.4K.
    WriteRegShift(UART0, UART0_REGSHIFT, 1, 0x00);

    // leave set-baud mode,
    // and set word length to 8 bits, no parity.
    WriteRegShift(UART0, UART0_REGSHIFT, LCR, LCR_EIGHT_BITS);

    // reset and enable FIFOs.
    WriteRegShift(UART0, UART0_REGSHIFT, FCR, FCR_FIFO_ENABLE | FCR_FIFO_CLEAR);

    // enable transmit and receive interrupts.
    WriteRegShift(UART0, UART0_REGSHIFT, IER, IER_TX_ENABLE | IER_RX_ENABLE);

    initlock(&uart_tx_lock, "uart");
}

// add a character to the output buffer and tell the
// UART to start sending if it isn't already.
// blocks if the output buffer is full.
// because it may block, it can't be called
// from interrupts; it's only suitable for use
// by write().
void uartputc(int c) {
    acquire(&uart_tx_lock);

    if (panicked) {
        for (;;)
            ;
    }
    while (uart_tx_w == uart_tx_r + UART_TX_BUF_SIZE) {
        // buffer is full.
        // wait for uartstart() to open up space in the buffer.
        sleep(&uart_tx_r, &uart_tx_lock);
    }
    uart_tx_buf[uart_tx_w % UART_TX_BUF_SIZE] = c;
    uart_tx_w += 1;
    uartstart();
    release(&uart_tx_lock);
}

// alternate version of uartputc() that doesn't
// use interrupts, for use by kernel printf() and
// to echo characters. it spins waiting for the uart's
// output register to be empty.
void uartputc_sync(int c) {
    push_off();

    if (panicked) {
        for (;;)
            ;
    }

    // wait for Transmit Holding Empty to be set in LSR.
    while ((ReadRegShift(UART0, UART0_REGSHIFT, LSR) & LSR_TX_IDLE) == 0)
        ;
    WriteRegShift(UART0, UART0_REGSHIFT, THR, c);

    pop_off();
}

// if the UART is idle, and a character is waiting
// in the transmit buffer, send it.
// caller must hold uart_tx_lock.
// called from both the top- and bottom-half.
void uartstart() {
    while (1) {
        if (uart_tx_w == uart_tx_r) {
            // transmit buffer is empty.
            ReadRegShift(UART0, UART0_REGSHIFT, ISR);
            return;
        }

        if ((ReadRegShift(UART0, UART0_REGSHIFT, LSR) & LSR_TX_IDLE) == 0) {
            // the UART transmit holding register is full,
            // so we cannot give it another byte.
            // it will interrupt when it's ready for a new byte.
            return;
        }

        int c = uart_tx_buf[uart_tx_r % UART_TX_BUF_SIZE];
        uart_tx_r += 1;

        // maybe uartputc() is waiting for space in the buffer.
        wakeup(&uart_tx_r);

        WriteRegShift(UART0, UART0_REGSHIFT, THR, c);
    }
}

// read one input character from the UART.
// return -1 if none is waiting.
int uartgetc(void) {
    if (ReadRegShift(UART0, UART0_REGSHIFT, LSR) & 0x01) {
        // input data is ready.
        return ReadRegShift(UART0, UART0_REGSHIFT, RHR);
    } else {
        return -1;
    }
}

// handle a uart interrupt, raised because input has
// arrived, or the uart is ready for more output, or
// both. called from devintr().
void uartintr(void) {
    // read and process incoming characters.
    while (1) {
        int c = uartgetc();
        if (c == -1)
            break;
        consoleintr(c);
    }

    // send buffered characters.
    acquire(&uart_tx_lock);
    uartstart();
    release(&uart_tx_lock);
}
