#ifndef SBI_H
#define SBI_H

#include "types.h"

struct sbiret sbi_ecall(int, int, uint64, uint64, uint64, uint64, uint64,
                        uint64);
void sbi_hart_start(uint64, uint64, uint64);
void startothers(int);
uint64 sbi_reboot(void);
void sbi_set_timer(uint64);

#endif // SBI_H
