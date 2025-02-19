#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

//
// the riscv Platform Level Interrupt Controller (PLIC).
//

void
plicinit(void)
{
  // set desired IRQ priorities non-zero (otherwise disabled).
  *(uint32*)(PLIC + UART0_IRQ*4) = 1;
}

void
plicinithart(void)
{
  int hart = cpuid();

  // set enable bits for this hart's S-mode
  #ifdef CONFIG_JH7110
  *(plic_t*)PLIC_SENABLE(hart) = (1ULL << UART0_IRQ);
   #else
  *(plic_t*)PLIC_SENABLE(hart) = (1 << UART0_IRQ);
  #endif

  // set this hart's S-mode priority threshold to 0.
  *(plic_t*)PLIC_SPRIORITY(hart) = 0;
}

// ask the PLIC what interrupt we should serve.
int
plic_claim(void)
{
  int hart = cpuid();
  int irq = *(plic_t*)PLIC_SCLAIM(hart);
  return irq;
}

// tell the PLIC we've served this IRQ.
void
plic_complete(int irq)
{
  int hart = cpuid();
  *(plic_t*)PLIC_SCLAIM(hart) = irq;
}
