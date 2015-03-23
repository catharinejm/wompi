#ifndef _MEM_H_
#define _MEM_H_

#define NULL ((void*)0)

#define PUT32(addr, val)                          \
    *((unsigned int*)addr) = (unsigned int)(val)

#define PUT16(addr, val)                              \
    *((unsigned short*)addr) = (unsigned short)(val)

#define PUT8(addr, val)                             \
    *((unsigned char*)addr) = (unsigned char)(val)

#define GET32(addr) \
    (*(unsigned int*)(addr))

#define barrier(...) \
    asm volatile("" ::: "memory")
#define dummy barrier


#endif
