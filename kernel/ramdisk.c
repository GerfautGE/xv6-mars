#include <kernel/buf.h>
#include <kernel/fs.h>
#include <kernel/memlayout.h>
#include <kernel/spinlock.h>
#include <kernel/string.h>

struct disk {
    char free[8];    // is a descriptor free?
    uint16 used_idx; // we've looked this far in used[2..NUM].
    struct spinlock rdisk_lock;
} disk;

void ramdiskinit(void) {
    initlock(&disk.rdisk_lock, "ramdisk_lock");
};

/**
 * @brief Read/write to the ramdisk
 * @param buf The buffer to read/write (see kernel/buf.h)
 * @param write 1 if write, 0 if read
 */
void ramdiskrw(struct buf *buf, int write) {
    acquire(&disk.rdisk_lock);
    // get the data of size BSIZE from the ramdisk at
    // RAMDISK + BSIZE * buf->blockno
    uint64 addr = RAMDISK + BSIZE * buf->blockno;
    if (write) {
        // write the data to the ramdisk
        memmove((uchar *)addr, buf->data, BSIZE);
    } else {
        // read the data from the ramdisk
        memmove(buf->data, (uchar *)addr, BSIZE);
    }
    release(&disk.rdisk_lock);
};
