#include "tty.h"
#include "mem.h"
#include "mailbox.h"

void init_tty(void) {
    fbinfo.width = 640;
    fbinfo.height = 480;
    fbinfo.virt_width = 640;
    fbinfo.virt_height = 480;
    fbinfo.pitch = 0;
    fbinfo.bit_depth = 24;
    fbinfo.x = 0;
    fbinfo.y = 0;
    fbinfo.ptr = NULL;
    fbinfo.size = 0;

    while (1) {
        mailbox_write(&fbinfo, 1);
        if (mailbox_read(1) == 1)
            if (fbinfo.ptr)
                break;
    }


    unsigned char *ltr = letters+0x36; // i got the BMP offset manually :-/
    unsigned char *framebuffer = (unsigned char*)fbinfo.ptr - COREVID_OFFSET;
    int rowlen = fbinfo.width * fbinfo.bit_depth / 3;
    int ltr_rowlen = 128*3;

    for (int i = 0; i < fbinfo.size; i++)
        framebuffer[i] = 0;

    for (int i = 0; i < 128; i++) {
        for (int j = i*rowlen; j < i+ltr_rowlen; j++)
            framebuffer[j] = *ltr++;
    }
    
}
