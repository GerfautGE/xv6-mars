#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "types.h"

#define WDT_LOAD 0x00
#define WDT_CTRL 0x008    // 0: reset enable
		          // 1: interrupt enable && watchdog enable
#define WDT_INT_CLR 0x00C
#define WDT_UNLOCK 0xC00

#define WDT_TIMEOUT 10
#define WDT_FREQ 24000000

#define JH7110_WDT_UNLOCK_KEY 0x1acce551

void watchdog_init();
void watchdog_feed();

#endif // WATCHDOG_H
