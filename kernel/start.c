#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

void main();
void timerinit();

// entry.S needs one stack per CPU.
__attribute__ ((aligned (16))) char stack0[4096 * NCPU];

// entry.S jumps here in machine mode on stack0.
void
start()
{
  // disable paging for now.
  sfence_vma();
  w_satp(0);
  sfence_vma();

  // disable interrupts and device interrupts.
  w_sie(0);
  w_sie(r_sstatus() & ~SSTATUS_SIE);

  // ask for clock interrupts.
  timerinit();

  // enable external interrupts.
  w_sie(r_sie() | SIE_SEIE | SIE_STIE | SIE_SSIE);

  main();
}

// ask each hart to generate timer interrupts
// TODO: this sould be in sbi.c
void
timerinit()
{
  // Setup SBI timer interrupt
  asm volatile("li a7, 0x54494D45");
  asm volatile("li a6, 0");
  // pass r_time()+1000000 to a0
  uint64 time = r_time() + 1000000;
  asm volatile("mv a0, %0" : : "r" (time));
  asm volatile("ecall");
}
