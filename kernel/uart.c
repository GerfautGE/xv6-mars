#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"


// SBI legacy console output
void
sbi_console_putchar(int ch)
{
  asm volatile("mv a0, %0" : : "r" (ch) :);
  asm volatile("li a6, 0");
  asm volatile("li a7, 0x01; ecall");
}
