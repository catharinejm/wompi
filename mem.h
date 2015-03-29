#ifndef _MEM_H_
#define _MEM_H_

#define NULL ((void*)0)

#define PUT32(addr, val)                                              \
    asm volatile("str %0, [%1]" :: "r" (val), "r" (addr) : "memory")

#define PUT16(addr, val)                                                \
    asm volatile("strh %0, [%1]" :: "r" (val), "r" (addr) : "memory")

#define PUT8(addr, val)                                                 \
    asm volatile("strb %0, [%1]" :: "r" (val), "r" (addr) : "memory")

#define GET32(addr, out)                                    \
    asm volatile("ldr %0, [%1]" : "=r" (out) : "r" (addr))

#define barrier(...) \
    asm volatile("" ::: "memory")
#define dummy barrier


#endif
