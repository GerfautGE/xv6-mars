#ifndef THERMAL_H
#define THERMAL_H

#define SYSCLK 0x13030000

#define THERMAL 0X120E0000
#define THERMAL_IRQ 81

#define JH7110_SYSCLK_TEMP_APB			129 // Bus
#define JH7110_SYSCLK_TEMP_CORE			130 // Sense

#define SFCTEMP_RSTN (1 << 0) // 0: reset,    1: de-assert
#define SFCTEMP_PD   (1 << 1) // 0: power up, 1: power down
#define SFCTEMP_RUN  (1 << 2) // 0: disable,  1: enable

#define SFCTEMP_DOUT_MSK    0xFFF0000
#define SFCTEMP_DOUT_POS    16
/* DOUT to Celcius conversion constants */
#define SFCTEMP_Y1000	237500L
#define SFCTEMP_Z	4094L
#define SFCTEMP_K1000	81100L

void thermal_init();

#endif // THERMAL_H
