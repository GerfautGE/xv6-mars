/*
 * Watchdog driver for JH7110
 * Author: GerfautGE
 */
#include <kernel/io.h>
#include <kernel/memlayout.h>
#include <kernel/syscrg.h>
#include <kernel/spinlock.h>
#include <kernel/printf.h>
#include <kernel/watchdog.h>


static int watchdog_init_clock_rst(){
    // enable clock
    struct spinlock wdt_lock;
    initlock(&wdt_lock, "watchdog");

    clk_enable(SYSCLK_WDT_APB, &wdt_lock);
    clk_enable(SYSCLK_WDT_CORE, &wdt_lock);

    rst_deassert(RSTN_WDT_APB, &wdt_lock);
    rst_deassert(RSTN_WDT_CORE, &wdt_lock);

    return 0;
}

// https://github.com/torvalds/linux/blob/master/drivers/watchdog/starfive-wdt.c#L343
void watchdog_init(){
	uint32 val = 0;

	// init clocks and resets
	watchdog_init_clock_rst();

	// Unlock Watchdog
	WriteReg32(WATCHDOG, WDT_UNLOCK, JH7110_WDT_UNLOCK_KEY);

	// Enable reset
	val = ReadReg32(WATCHDOG, WDT_CTRL);
	val |= 0x1 << 0x1; // rst shift
	WriteReg(WATCHDOG, WDT_CTRL, val);
	
	// Clear interrupt signal before init
	WriteReg(WATCHDOG, WDT_INT_CLR, 0x1);

	// Set count
	uint32 count = (WDT_TIMEOUT * WDT_FREQ)/2; // Double timeout
	WriteReg32(WATCHDOG, WDT_LOAD, count);

	// Enable
	val = ReadReg32(WATCHDOG, WDT_CTRL);
	val |= 0x1 << 0x0; // en shift
	WriteReg(WATCHDOG, WDT_CTRL, val);
}

void watchdog_feed(){
	uint32 count = (WDT_TIMEOUT * WDT_FREQ)/2; // Double timeout
	WriteReg32(WATCHDOG, WDT_LOAD, count);
}
