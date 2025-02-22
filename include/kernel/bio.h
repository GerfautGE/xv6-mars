#ifndef BIO_H
#define BIO_H

#include "types.h"

void binit(void);
struct buf *bread(uint, uint);
void brelse(struct buf *);
void bwrite(struct buf *);
void bpin(struct buf *);
void bunpin(struct buf *);

#endif // BIO_H
