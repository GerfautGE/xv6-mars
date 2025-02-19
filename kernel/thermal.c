#ifdef CONFIG_JH7110

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

  // Associate the file structure with the device
  devsw[TEMP].read = thermalread;
  devsw[TEMP].write = 0;
}

int thermalread(int user_dst, uint64 dst, int n){
  uint32 temp;
  uint32 dout = *((volatile uint32 *)(THERMAL)) & SFCTEMP_DOUT_MSK;
  dout >>= SFCTEMP_DOUT_POS; // Shift to get the output value
  // Calculate the temperature in millidegrees Celsius (mC)
  temp = dout * SFCTEMP_Y1000 / SFCTEMP_Z - SFCTEMP_K1000;

  //convert to string
  char temp_str[7];
  temp_str[5] = '\n';
  temp_str[6] = '\0';
  for(int i = 4; i >= 0; i--){
    temp_str[i] = (temp % 10) + '0';
    temp /= 10;
  }
  static int offset = 0;               // Track read position

    int len = sizeof(temp_str) - 1;  // Exclude null terminator

    if (offset >= len) {
      offset = 0;  // Reset for next `cat temp`
      return 0;    // EOF, tell `cat` to stop
    }

    // Compute how much remains to send
    int remaining = len - offset;
    int to_copy = (n < remaining) ? n : remaining;

    // Copy to user
    if (either_copyout(user_dst, dst, temp_str + offset, to_copy) == -1)
      return -1;

    offset += to_copy;
    return to_copy; // Return bytes written
}

#endif
