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
void
timerinit()
{
  sbi_set_timer(1000000);
}
