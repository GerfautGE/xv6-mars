#ifndef TYPES_H
#define TYPES_H

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;

typedef uint64 pde_t;

#if defined(CONFIG_MARS)
typedef uint64 plic_t; // Si CONFIG_MARS est défini, utilise uint64
#else
typedef uint32 plic_t; // Sinon, utilise uint32
#endif

#endif
