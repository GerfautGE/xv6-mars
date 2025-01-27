/*
 * MilkV Mars SOC Memory Layout
 * https://doc-en.rvspace.org/JH7110/TRM/JH7110_TRM/u74_memory_map.html
 *
 * Author: Luc Videau
 */


// 00001000 -- boot ROM, provided by qemu
// 02000000 -- CLINT
// 0C000000 -- PLIC
// 40000000 -- OpenSBI Firmware
// 40200000 -- boot ROM jumps here in machine mode
//             -kernel loads the kernel here
// 49000000 -- ramdisk

// the kernel uses physical memory thus:
// 40200000 -- entry.S, then kernel text and data
// end -- start of kernel page allocation area
// PHYSTOP -- end RAM used by the kernel

// qemu puts platform-level interrupt controller (PLIC) here.
#define PLIC 0x0c000000L
#define PLIC_PRIORITY (PLIC + 0x0)
#define PLIC_PENDING (PLIC + 0x1000)
#define PLIC_SENABLE(hart) (PLIC + 0x2080 + (hart)*0x100)
#define PLIC_SPRIORITY(hart) (PLIC + 0x201000 + (hart)*0x2000)
#define PLIC_SCLAIM(hart) (PLIC + 0x201004 + (hart)*0x2000)

// the kernel expects there to be RAM
// for use by the kernel and user pages
// from physical address 0x40000000 to PHYSTOP.
#define KERNBASE 0x40200000L

//https://lupyuen.codeberg.page/articles/privilege.html#wait-forever-in-uart-transmit
#define UART0_OFFSET 4

// https://lupyuen.org/articles/plic.html
#define UART0_IRQ 27

// ramdisk interface
#define RAMDISK 0x49000000L
