#ifndef IO_H
#define IO_H

#include <kernel/types.h>

#define Reg(reg) ((volatile unsigned char *)((reg)))

#define ReadRegShift(base, shift, reg) (*(Reg(base + (reg << shift))))
#define WriteRegShift(base, shift, reg, v) (*(Reg(base + (reg << shift))) = (v))

#define ReadReg(base, reg) (*(Reg(base + reg)))
#define WriteReg(base, reg, v) (*(Reg(base + reg)) = (v))

#define ReadReg32(base, reg) (*(volatile uint32 *)((uint64)(base) + (reg)))
#define WriteReg32(base, reg, v) (*(volatile uint32 *)((uint64)(base) + (reg)) = (v))

#define ReadRegShift32(base, shift, reg) (*(volatile uint32 *)((uint64)(base) + ((reg) << (shift))))
#define WriteRegShift32(base, shift, reg, v) (*(volatile uint32 *)((uint64)(base) + ((reg) << (shift))) = (v))

#endif // IO_H
