#ifndef FS_H
#define FS_H

#include "file.h"
#include "param.h"
#include "stat.h"
#include "types.h"

// On-disk file system format.
// Both the kernel and user programs use this header file.
#define ROOTINO 1  // root i-number
#define BSIZE 1024 // block size

// Disk layout:
// [ boot block | super block | log | inode blocks |
//                                          free bit map | data blocks]
//
// mkfs computes the super block and builds an initial file system. The
// super block describes the disk layout:
struct superblock {
    uint magic;      // Must be FSMAGIC
    uint size;       // Size of file system image (blocks)
    uint nblocks;    // Number of data blocks
    uint ninodes;    // Number of inodes.
    uint nlog;       // Number of log blocks
    uint logstart;   // Block number of first log block
    uint inodestart; // Block number of first inode block
    uint bmapstart;  // Block number of first free map block
};

#define FSMAGIC 0x10203040

#define NINDIRECT (BSIZE / sizeof(uint))
#define MAXFILE (NDIRECT + NINDIRECT)

// On-disk inode structure
struct dinode {
    short type;              // File type
    short major;             // Major device number (T_DEVICE only)
    short minor;             // Minor device number (T_DEVICE only)
    short nlink;             // Number of links to inode in file system
    uint size;               // Size of file (bytes)
    uint addrs[NDIRECT + 1]; // Data block addresses
};

// Inodes per block.
#define IPB (BSIZE / sizeof(struct dinode))

// Block containing inode i
#define IBLOCK(i, sb) ((i) / IPB + sb.inodestart)

// Bitmap bits per block
#define BPB (BSIZE * 8)

// Block of free map containing bit for block b
#define BBLOCK(b, sb) ((b) / BPB + sb.bmapstart)

// Directory is a file containing a sequence of dirent structures.
#define DIRSIZ 14

struct dirent {
    ushort inum;
    char name[DIRSIZ];
};

void fsinit(int);
int dirlink(struct inode *, char *, uint);
struct inode *dirlookup(struct inode *, char *, uint *);
struct inode *ialloc(uint, short);
struct inode *idup(struct inode *);
void iinit();
void ilock(struct inode *);
void iput(struct inode *);
void iunlock(struct inode *);
void iunlockput(struct inode *);
void iupdate(struct inode *);
int namecmp(const char *, const char *);
struct inode *namei(char *);
struct inode *nameiparent(char *, char *);
int readi(struct inode *, int, uint64, uint, uint);
void stati(struct inode *, struct stat *);
int writei(struct inode *, int, uint64, uint, uint);
void itrunc(struct inode *);

#endif // FS_H
