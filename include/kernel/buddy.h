#ifndef BUDDY_H
#define BUDDY_H

#include "types.h"

void           bd_init(void*,void*);
void           bd_free(void*);
void           *bd_malloc(uint64);

#endif // BUDDY_H
