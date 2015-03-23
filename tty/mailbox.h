#ifndef _MAILBOX_H_
#define _MAILBOX_H_

#include "bcm2806.h"

#define MAILBOX_BASE PERI_BASE + 0xB880
#define MAIL0_READ MAILBOX_BASE + 0x00
#define MAIL0_PEEK MAILBOX_BASE + 0x10
#define MAIL0_SENDER MAILBOX_BASE + 0x14
#define MAIL0_STATUS MAILBOX_BASE + 0x18
#define MAIL0_CONFIG MAILBOX_BASE + 0x1C
#define MAIL0_WRITE MAILBOX_BASE + 0x20 // Also MAIL1_READ

#define COREVID_OFFSET 0xC0000000

#define MAIL_FULL (1 << 31)
#define MAIL_EMPTY (1 << 30)

struct fb_info {
    unsigned int width,
        height,
        virt_width,
        virt_height,
        pitch,
        bit_depth,
        x,
        y;
    void *ptr;
    unsigned int size;
};

extern volatile struct fb_info fbinfo;
extern unsigned char *letters;

int mailbox_write(volatile struct fb_info* fbinfo, unsigned int channel);
unsigned int mailbox_read(unsigned int channel);


#endif
