#ifndef SPINLOCK_H
#define SPINLOCK_H

#include "types.h"
// Mutual exclusion lock.
struct spinlock {
    uint locked; // Is the lock held?

    // For debugging:
    char *name;      // Name of lock.
    struct cpu *cpu; // The cpu holding the lock.
    uint64 pc;
    int pid;
    uint n;
    uint nts;
};

void acquire(struct spinlock *);
int holding(struct spinlock *);
void initlock(struct spinlock *, char *);
void release(struct spinlock *);
void push_off(void);
void pop_off(void);

#endif // SPINLOCK_H
