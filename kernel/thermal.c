#ifdef CONFIG_MARS

#include "thermal.h"
#include "io.h"
#include "types.h"

extern void printf(char *s, ...);


void thermal_init() {
  WriteReg(THERMAL, 0, SFCTEMP_PD); // Power down the thermal sensor
  WriteReg(THERMAL, 0, 0); // Power Up
  WriteReg(THERMAL, 0, SFCTEMP_RSTN); // De-assert reset

  // enable thermal sensor
  WriteReg(THERMAL, 0, SFCTEMP_RUN);

  uint32 dout = *((volatile uint32 *)(THERMAL)) & SFCTEMP_DOUT_MSK;
  dout >>= SFCTEMP_DOUT_POS; // Shift to get the output value
  // Calculate the temperature in millidegrees Celsius (mC)
  long temp = (long)dout * SFCTEMP_Y1000 / SFCTEMP_Z - SFCTEMP_K1000;

  printf("Thermal sensor initialized. Temperature: %ld mC\n", temp);
}

#endif
