#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

volatile static int started = 0;
volatile static int boot_hartid = -1;

struct sbiret
{
    long error;
    long value;
};

struct sbiret sbi_ecall(int ext, int fid, uint64 arg0, uint64 arg1,
                        uint64 arg2, uint64 arg3, uint64 arg4, uint64 arg5)
{
    struct sbiret ret;

    register uint64 a0 asm("a0") = arg0;
    register uint64 a1 asm("a1") = arg1;
    register uint64 a2 asm("a2") = arg2;
    register uint64 a3 asm("a3") = arg3;
    register uint64 a4 asm("a4") = arg4;
    register uint64 a5 asm("a5") = arg5;
    register uint64 a6 asm("a6") = fid;
    register uint64 a7 asm("a7") = ext;
    asm volatile("ecall"
                 : "+r"(a0), "+r"(a1)
                 : "r"(a2), "r"(a3), "r"(a4), "r"(a5), "r"(a6), "r"(a7)
                 : "memory");

    ret.error = a0;
    ret.value = a1;

    return ret;
}

static inline void sbi_hart_start(uint64 hartid, uint64 saddr, uint64 priv)
{
  sbi_ecall(0x48534D, 0x0, hartid, saddr, priv, 0,
                    0, 0);
}

void startothers(void){
  if (boot_hartid == -1 ){
   exit(1);
  }
  for (int i = 0; i < NCPU; i++)
  {
    if (i == boot_hartid)
      continue;
    sbi_hart_start(i, (uint64)KERNBASE, 1);
  }

}

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
    startothers(); // boot other CPUs
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
