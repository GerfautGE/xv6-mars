/**
 * This is a GMAC driver for JH7110 Milkv MARS
 * Author: GerfautGE
 */

#include <kernel/memlayout.h>
#include <kernel/printf.h>

// https://github.com/torvalds/linux/blob/10d97b74e2eef787d823f9bc05cb044b47d25c41/drivers/net/ethernet/stmicro/stmmac/dwmac4.h

// https://pdos.csail.mit.edu/6.1810/2025/labs/net.html

#define MAC_HIGH 0x300
#define MAC_LOW 0x304

#ifdef CONFIG_JH7110
void gmac_init(){
	// reset GMAC controller (clocks + deassert)

	// read MAC addr from hardware
	volatile unsigned int *reg = (volatile unsigned int *) (GMAC0_BASE + MAC_HIGH);
	unsigned int high = *reg;
	volatile unsigned int *reg_lo = (volatile unsigned int *) (GMAC0_BASE + MAC_LOW);
	unsigned int low = *reg_lo;
	printf("MAC: %x:%x\n", high, low); 

	// set MAC ?
}
#endif
