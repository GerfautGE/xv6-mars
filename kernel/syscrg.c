#ifdef CONFIG_JH7110
#include <kernel/memlayout.h>
#include <kernel/printf.h>
#include <kernel/spinlock.h>
#include <kernel/syscrg.h>

void clk_enable(int num_clk, struct spinlock *lock) {
    volatile uint32 *addr = (volatile uint32 *)(SYSCRG + 4 * num_clk);
    // ensure that the address is within the valid range
    if (addr < (volatile uint32 *)SYSCRG ||
        addr > (volatile uint32 *)(SYSCRG + SYSCRG_RANGE)) {
        panic("clk_enable: invalid address");
    }
    acquire(lock);
    *addr |= CLK_ENABLE;
    release(lock);
}

void rst_deassert(int num_rst, struct spinlock *lock) {
    volatile uint32 *addr =
        (volatile uint32 *)(SYSCRG + 4 * (RSTN_BASE + num_rst / 32));
    // ensure that the address is within the valid range
    if (addr < (volatile uint32 *)SYSCRG ||
        addr > (volatile uint32 *)(SYSCRG + SYSCRG_RANGE)) {
        panic("rst_deassert: invalid address");
    }
    acquire(lock);
    // set only the id%32 bit to 0
    *addr &= ~(1 << (num_rst % 32));
    release(lock);
}
#endif
