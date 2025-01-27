#ifndef MEMORY_LAYOUT_H
#define MEMORY_LAYOUT_H

#if defined(QEMU)
// QEMU virt
#include "memlayout-qemu.h"
#else
// MilkV Mars
#include "memlayout-mars.h"
#endif

// Common definitions for all memory layouts.

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
