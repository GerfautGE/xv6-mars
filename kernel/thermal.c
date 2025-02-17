#ifdef CONFIG_MARS

#include "memlayout.h"
#include "thermal.h"
#include "io.h"
#include "types.h"
#include "syscrg.h"
#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "riscv.h"
#include "defs.h"
#include "proc.h"
#include "stat.h"


void thermal_init() {
  // enable clock
  struct spinlock temp_lock;
  initlock(&temp_lock, "temp");

  clk_enable(SYSCLK_TEMP_APB, &temp_lock);
  clk_enable(SYSCLK_TEMP_CORE, &temp_lock);

  rst_deassert(RSTN_TEMP_APB, &temp_lock);
  rst_deassert(RSTN_TEMP_CORE, &temp_lock);

  WriteReg(THERMAL, 0, SFCTEMP_PD); // Power down the thermal sensor
  WriteReg(THERMAL, 0, 0); // Power Up
  WriteReg(THERMAL, 0, SFCTEMP_RSTN); // De-assert reset

  // enable thermal sensor
  WriteReg(THERMAL, 0, SFCTEMP_RUN | SFCTEMP_RSTN);
}

int thermalread(){
  uint32 dout = *((volatile uint32 *)(THERMAL)) & SFCTEMP_DOUT_MSK;
  dout >>= SFCTEMP_DOUT_POS; // Shift to get the output value
  // Calculate the temperature in millidegrees Celsius (mC)
  long temp = (long)dout * SFCTEMP_Y1000 / SFCTEMP_Z - SFCTEMP_K1000;
  return temp;
}

#endif
