#ifndef SLEEPLOCK_H
#define SLEEPLOCK_H

#include "spinlock.h"
#include "types.h"

// Long-term locks for processes
struct sleeplock {
    uint locked;        // Is the lock held?
    struct spinlock lk; // spinlock protecting this sleep lock

    // For debugging:
    char *name; // Name of lock.
    int pid;    // Process holding lock
};
#endif // SLEEPLOCK_H

void acquiresleep(struct sleeplock *);
void releasesleep(struct sleeplock *);
int holdingsleep(struct sleeplock *);
void initsleeplock(struct sleeplock *, char *);
