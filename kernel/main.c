#include <kernel/proc.h>
#include <kernel/console.h>
#include <kernel/printf.h>
#include <kernel/vm.h>
#include <kernel/ramdisk.h>
#include <kernel/uart.h>
#include <kernel/trap.h>
#include <kernel/plic.h>
#include <kernel/buf.h>
#include <kernel/sbi.h>
#include <kernel/bio.h>
#include <kernel/kalloc.h>
#include <kernel/thermal.h>

volatile static int started = 0;
volatile static int boot_hartid = -1;

// start() jumps here in supervisor mode on all CPUs.
void
main()
{
  if (boot_hartid == -1) {
    boot_hartid = cpuid(); // tell others that I'm the boot CPU
    consoleinit();
    printfinit();
    printf("\nxv6 kernel is booting\n\n");
    printf("hart %d starting\n", boot_hartid);
    kinit();         // physical page allocator
    kvminit();       // create kernel page table
    startothers(boot_hartid);   // boot other CPUs
    kvminithart();   // turn on paging
    procinit();      // process table
    trapinit();      // trap vectors
    trapinithart();  // install kernel trap vector
    plicinit();      // set up interrupt controller
    plicinithart();  // ask PLIC for device interrupts
    binit();         // buffer cache
    iinit();         // inode table
    fileinit();      // file table
    ramdiskinit();   // ramdisk initialisation
    #ifdef CONFIG_JH7110
    thermal_init();  // thermal sensor initialisation
    #endif
    userinit();      // first user process
    __sync_synchronize();
    started = 1;
  } else {
    while(started == 0)
      ;
    __sync_synchronize();
    printf("hart %d starting\n", cpuid());
    kvminithart();    // turn on paging
    trapinithart();   // install kernel trap vector
    plicinithart();   // ask PLIC for device interrupts
  }

  scheduler();
}
