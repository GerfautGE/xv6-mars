#ifndef MEMORY_LAYOUT_H
#define MEMORY_LAYOUT_H

#ifdef CONFIG_MARS

#define KERNBASE 0x40200000L
//https://lupyuen.codeberg.page/articles/privilege.html#wait-forever-in-uart-transmit
#define UART0_OFFSET 4
// https://lupyuen.org/articles/plic.html
#define UART0_IRQ 27
// ramdisk interface
#define RAMDISK 0x49000000L

#endif
#ifdef CONFIG_QEMU

#define KERNBASE 0x80200000L
#define UART0_OFFSET 1
#define UART0_IRQ 10
#define RAMDISK 0x89000000L

#endif


// Common definitions for all memory layouts.

// qemu puts platform-level interrupt controller (PLIC) here.
#define PLIC 0x0c000000L
#define PLIC_PRIORITY (PLIC + 0x0)
#define PLIC_PENDING (PLIC + 0x1000)
#define PLIC_SENABLE(hart) (PLIC + 0x2080 + (hart)*0x100)
#define PLIC_SPRIORITY(hart) (PLIC + 0x201000 + (hart)*0x2000)
#define PLIC_SCLAIM(hart) (PLIC + 0x201004 + (hart)*0x2000)

// UART registers are at 0x10000000 on the Mars and Qemu virt platforms
#define UART0 0x10000000

#define PHYSTOP (KERNBASE + 128*1024*1024)

// map the trampoline page to the highest address,
// in both user and kernel space.
#define TRAMPOLINE (MAXVA - PGSIZE)

// map kernel stacks beneath the trampoline,
// each surrounded by invalid guard pages.
#define KSTACK(p) (TRAMPOLINE - ((p)+1)* 2*PGSIZE)

// User memory layout.
// Address zero first:
//   text
//   original data and bss
//   fixed-size stack
//   expandable heap
//   ...
//   TRAPFRAME (p->trapframe, used by the trampoline)
//   TRAMPOLINE (the same page as in the kernel)
#define TRAPFRAME (TRAMPOLINE - PGSIZE)

#endif // MEMORY_LAYOUT_H
