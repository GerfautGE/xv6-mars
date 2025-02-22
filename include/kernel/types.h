#ifndef TYPES_H
#define TYPES_H

typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int  uint32;
typedef unsigned long uint64;

typedef uint64 pde_t;
typedef uint64 pte_t;
typedef uint64 *pagetable_t; // 512 PTEs


#if defined(CONFIG_JH7110)
    typedef uint64 plic_t;  // Si CONFIG_JH7110 est défini, utilise uint64
#else
    typedef uint32 plic_t;  // Sinon, utilise uint32
#endif

#endif
