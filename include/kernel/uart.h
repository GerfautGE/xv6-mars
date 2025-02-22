#ifndef UART_H
#define UART_H

void uartinit(void);
void uartintr(void);
void uartputc(int);
void uartputc_sync(int);
int uartgetc(void);

#endif // UART_H
