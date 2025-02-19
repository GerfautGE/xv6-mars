#ifndef SYSCRG_H
#define SYSCRG_H

#include "types.h"
struct spinlock;

/* register fields */
#define CLK_ENABLE	(1 << 31)

//https://doc-en.rvspace.org/JH7110/TRM/JH7110_TRM/sys_crg.html#sys_crg__section_v3t_pfn_wsb

/* JH7110 clocks */
#define SYSCLK_TEMP_APB			  129 // Bus
#define SYSCLK_TEMP_CORE			130 // Sense

/* JH7110 resets */
#define RSTN_BASE         190 // End of the clock list
// to assert or deassert a reset, write a 1 or 0 to the corresponding bit
#define RSTN_TEMP_APB		  123 // Bus
#define RSTN_TEMP_CORE		124 // Sense

/* Prototypes */
void clk_enable(int num_clk, struct spinlock *lock);
void rst_deassert(int num_rst, struct spinlock *lock);

#endif // SYSCRG_H
