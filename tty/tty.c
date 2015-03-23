#include "tty.h"
#include "mem.h"
#include "mailbox.h"

void init_tty(void) {
    volatile struct fb_info fbinfo __attribute__ ((aligned (16))) = {
        .width = 640,
        .height = 480,
        .virt_width = 640,
        .virt_height = 480,
        .pitch = 0,
        .bit_depth = 24,
        .x = 0,
        .y = 0,
        .ptr = NULL,
        .size = 0,
    };
    
    while (1) {
        mailbox_write(&fbinfo, 1);
        if (mailbox_read(1) == 1)
            if (fbinfo.ptr)
                break;
    }


    volatile unsigned char *ltr = letters+0x36; // i got the BMP offset manually :-/
    unsigned char *framebuffer = (unsigned char*)fbinfo.ptr - COREVID_OFFSET;
    int rowlen = fbinfo.width * fbinfo.bit_depth / 3;
    int ltr_rowlen = 128*3;

    for (int i = 0; i < fbinfo.size; i++)
        framebuffer[i] = 0;

    for (int i = 0; i < 128; i++) {
        for (int j = i*rowlen; j < i*rowlen+ltr_rowlen; j++)
            framebuffer[j] = *ltr++;
    }
    
}
