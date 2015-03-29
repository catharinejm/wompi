#include "mailbox.h"
#include "mem.h"

int mailbox_write(volatile struct fb_info *fbinfo, unsigned int channel) {
    // If address isnt 16-byte aligned we can't use it
    if (((unsigned int)fbinfo) & 0xF)
        return -1;

    while (1) {
        int status;
        barrier();
        GET32(MAIL0_STATUS, status);
        if ((status & MAIL_FULL) == 0)
            break;
    }
    unsigned val = (unsigned)fbinfo + COREVID_OFFSET + channel;
    PUT32(MAIL0_WRITE,val);
    return 0;
}

unsigned int mailbox_read(unsigned int channel) {
    unsigned int val = 0;
    while (1) {
        barrier();
        while (1) {
            barrier();
            int status;
            GET32(MAIL0_STATUS, status);
            if ((status & MAIL_EMPTY) == 0)
                break;
        }
        GET32(MAIL0_READ, val);
        if ((val & 0x0F) == channel) break;
    }
    return val;
}
