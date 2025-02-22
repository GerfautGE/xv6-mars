#ifndef RAMDISK_H
#define RAMDISK_H

#include "buf.h"

void ramdiskinit(void);
void ramdiskintr(void);
void ramdiskrw(struct buf *, int);

#endif // RAMDISK_H
