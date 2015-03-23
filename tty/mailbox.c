#include "mailbox.h"
#include "mem.h"

int mailbox_write(volatile struct fb_info *fbinfo, unsigned int channel) {
    unsigned int fbaddr;

    // If address isnt 16-byte aligned we can't use it
    if (((unsigned int)fbinfo) & 0xF)
        return -1;

    fbaddr = (((unsigned int)fbinfo) & ~0xF) | channel;
    while(GET32(MAIL0_STATUS) & MAIL_FULL)
        barrier();
    PUT32(MAIL0_WRITE,fbaddr);
    return 0;
}

unsigned int mailbox_read(unsigned int channel) {
    unsigned int val = 0;
    while ((val & 0xF) != channel) {
        while (GET32(MAIL0_STATUS) & MAIL_EMPTY)
            barrier();
        val = GET32(MAIL0_READ);
    }
    return val & ~0xF;
}
