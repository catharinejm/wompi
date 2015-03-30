#include "tty.h"
#include "mem.h"
#include "mailbox.h"
#include "types.h"

void tty_scroll(struct console *con) {
    volatile u32 *to = con->framebuffer32;
    union px_v from = { con->framebuffer + ROW_AREA(con) };
    while (from.p32 < con->fb_end32)
        (*to++) = *from.p32++;
    while (to < con->fb_end32)
        (*to++) = 0;
    con->row--;
}

void tty_write_str(struct console *con, char *str) {
    for (char *c = str; *c; c++)
        tty_write(con, *c);
}

/* assumes 8-bit color */
static inline void tty_paint_char(struct console *con, const void *let) {
    union px_v pix = { CHAR_TOPLEFT(con) };

    // 2 4-byte words written per line of character. pix is incremented after first write,
    // so pitch - 1 words remain after second write
    int row_incr = (con->fbinfo.pitch / 4) - 1;
    const u32* let32 = let;
    if (NULL == let) {
        for (int cnt = 0;cnt < CHAR_HEIGHT; cnt++) {
            (*pix.p32++) = 0;
            *pix.p32 = 0;
            pix.p32 += row_incr;
        }
    } else {
        for (int cnt = 0; cnt < CHAR_HEIGHT; cnt++) {
            (*pix.p32++) = *let32++;
            *pix.p32 = *let32++;
            pix.p32 += row_incr;
        }
    }
}

void tty_write(struct console *con, char c) {
    if (c >= '!' && c <= '~') {
        int cidx = c - '!';
        int coff = cidx*CHAR_SIZE;
        tty_paint_char(con, con->letters+coff);
        INC_COL(con);
    } else if (c == ' ' || c == '\t') { // one space for tabs, i guess...
        INC_COL(con);
    } else if (c == '\r' || c == '\n') {
        INC_ROW(con);
        tty_write_str(con, con->prompt);
    } else if (c == '\b' || c == 127) {
        if (0 < con->col) {
            con->col--;
            tty_paint_char(con, NULL);
        }
    }
}

void tty_clear(struct console *con) {
    union px_v fb = { con->framebuffer };
    while (fb.p32 < con->fb_end32)
        (*fb.p32++) = 0;
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
    con->fbinfo.cmap[0] = 0;
    con->fbinfo.cmap[255] = 0xFFFF;
    
    while (1) {
        mailbox_write(&con->fbinfo, 1);
        if (mailbox_read(1) == 1)
            if (con->fbinfo.ptr)
                break;
    }



    con->prompt = "> ";
    con->letters = letters;
    con->framebuffer = (u8*)con->fbinfo.ptr - COREVID_OFFSET;
    con->fb_end = con->framebuffer + con->fbinfo.size;
    con->row = 0;
    con->last_row = con->fbinfo.height / 15; // 15px is char height
    con->col = 2; // start after prompt
    con->last_col = con->fbinfo.width / 8; // 8px is char width


    tty_clear(con);
    tty_write_str(con, "Welcome to Wombat!\n");
}
