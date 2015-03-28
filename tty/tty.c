#include "tty.h"
#include "mem.h"
#include "mailbox.h"
#include "types.h"

void tty_scroll(struct console *con) {
    int row_len = con->fbinfo.width * PXWIDTH;
    int row_area = row_len * CHAR_HEIGHT;
    int i;
    for (i = 0; i < con->fbinfo.size - row_area; i++) {
        con->framebuffer[i] = con->framebuffer[i+row_area];
    }
    for (; i < con->fbinfo.size; i++) {
        con->framebuffer[i] = 0;
    }
    con->row--;
}

void tty_write_str(struct console *con, char *str) {
    for (char *c = str; *c; c++)
        tty_write(con, *c);
}

void tty_write(struct console *con, char c) {
    if (c >= '!' && c <= '~') {
        int cidx = c - '!';

        int fb_row_len = con->fbinfo.width * PXWIDTH;
        int fb_start = con->row*fb_row_len*CHAR_HEIGHT + con->col*CHAR_PXWIDTH;
        
        
        int coff = cidx*CHAR_SIZE;
        const byte_t *let = con->letters+coff;
        for (int i = 0; i < CHAR_HEIGHT; i++) {
            for (int j = 0; j < CHAR_PXWIDTH; j++)
                con->framebuffer[fb_start+j+i*fb_row_len] = *let++;
        }
        INC_COL(con);
    } else if (c == ' ' || c == '\t') { // one space for tabs, i guess...
        INC_COL(con);
    } else if (c == '\n') {
        INC_ROW(con);
        tty_write_str(con, con->prompt);
    }
}

void tty_clear(struct console *con) {
    for (int i = 0; i < con->fbinfo.size; i++)
        con->framebuffer[i] = 0;
    con->row=0;
    con->col=0;
    char *p = con->prompt;
    while(p && *p)
        tty_write(con, *p++);
}

void tty_init(struct console *con) {
    con->fbinfo.width = 640;
    con->fbinfo.height = 480;
    con->fbinfo.virt_width = 640;
    con->fbinfo.virt_height = 480;
    con->fbinfo.pitch = 0;
    con->fbinfo.bit_depth = BIT_DEPTH;
    con->fbinfo.x = 0;
    con->fbinfo.y = 0;
    con->fbinfo.ptr = NULL;
    con->fbinfo.size = 0;
    
    while (1) {
        mailbox_write(&con->fbinfo, 1);
        if (mailbox_read(1) == 1)
            if (con->fbinfo.ptr)
                break;
    }



    con->prompt = "> ";
    con->letters = letters;//+0x36;
    con->framebuffer = (byte_t*)con->fbinfo.ptr - COREVID_OFFSET;
    con->row = 0;
    con->last_row = con->fbinfo.height / 15; // 15px is char height
    con->col = 2; // start after prompt
    con->last_col = con->fbinfo.width / 8; // 8px is char width


    tty_clear(con);
    tty_write_str(con, "Welcome to Wombat!\n");
}
