#ifndef KALLOC_H
#define KALLOC_H

#include "memlayout.h"
#include "riscv.h"

void *kalloc(void);
void kfree(void *);
void kinit(void);

// references to ptes
extern int ref_count[PHYSTOP / PGSIZE];

#endif // KALLOC_H
