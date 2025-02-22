#include <kernel/types.h>
#include <kernel/param.h>
#include <kernel/memlayout.h>
#include <kernel/riscv.h>
#include <kernel/proc.h>

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

inline void sbi_hart_start(uint64 hartid, uint64 saddr, uint64 priv)
{
  sbi_ecall(0x48534D, 0x0, hartid, saddr, priv, 0,
                    0, 0);
}

void startothers(int boot_hartid)
{
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

uint64
sbi_reboot(void)
{
  // SBI call to sbi_system_reset(uint32_t reset_type, uint32_t reset_reason)
  uint32 reset_type = 0x1; // cold reboot
  uint32 reset_reason = 0x0; // no reason
  sbi_ecall(0x53525354, 0x0, reset_type, reset_reason, 0, 0, 0, 0);
  return 0;
}

void sbi_set_timer(uint64 delay){
  sbi_ecall(0x54494D45, 0x0, r_time() + delay, 0, 0, 0, 0, 0);
}
