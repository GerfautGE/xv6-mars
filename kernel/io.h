#ifndef IO_H
#define IO_H

#define ReadRegShift(base, shift, reg) (*(Reg(base+(reg<<shift))))
#define WriteRegShift(base, shift, reg, v) (*(Reg(base+(reg<<shift))) = (v))

#define ReadReg(base, reg) (*(Reg(base+reg)))
#define WriteReg(base, reg, v) (*(Reg(base+reg)) = (v))

#define Reg(reg) ((volatile unsigned char *)((reg)))

#endif // IO_H
