#ifndef _TYPES_H_
#define _TYPES_H_

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed		char		s8;
typedef signed		short		s16;
typedef signed		int			s32;
typedef signed		long long	s64;

typedef unsigned int size_t;

typedef enum {
	false = 0,
	true = 1,
} bool;

/*
 * Pointer eXtended -- unions for various bytelen aliases. Useful for
 * writing blocks to the framebuffer. Also comes in 'volatile' and
 * 'const' flavors.
 */
union px {
    u8 *p;
    u16 *p16;
    u32 *p32;
};

union px_v {
    volatile u8 *p;
    volatile u16 *p16;
    volatile u32 *p32;
};

union px_c {
    const u8 *p;
    const u16 *p16;
    const u32 *p32;
};

#endif
