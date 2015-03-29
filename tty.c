#include "tty.h"
#include "mem.h"
#include "mailbox.h"
#include "types.h"

void tty_scroll(struct console *con) {
    int row_len = con->fbinfo.width * PXWIDTH;
    int row_area = row_len * CHAR_HEIGHT;
    volatile u32 *to = con->framebuffer32;
    union px_v from = { con->framebuffer + row_area };
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
    int fb_start = con->row*con->fbinfo.pitch*CHAR_HEIGHT + con->col*CHAR_PXWIDTH;
    volatile u32 *pix = (u32*)(con->framebuffer + fb_start);
    int inc = (con->fbinfo.pitch - 4) >> 2;
    u32 cnt = 0;
    const u32* let32 = let;
    if (NULL == let) {
        for (;cnt < 15; cnt++) {
            (*pix++) = 0;
            *pix = 0;
            pix += inc;
        }
    } else {
        for (; cnt < 15; cnt++) {
            (*pix++) = *let32++;
            *pix = *let32++;
            pix += inc;
        }
    }
    // if (NULL == let) {
    //     asm volatile("mov r0, #0 \n"
    //                  "mov r1, #0"
    //                  ::: "r0", "r1");
    //     while (cnt < 15) {
    //         asm volatile ("stm %[pix], {r0,r1} \n"
    //                       "add %[pix], %[inc] \n"
    //                       "add %[cnt], #1 \n"
    //                       : [pix] "+r" (pix), [cnt] "+r" (cnt)
    //                       : [inc] "r" (inc)
    //                       : "memory", "r0", "r1");
    //     }
    // } else {
    //     while (cnt < 7) {
    //         asm volatile ("ldm %[let], {r0,r1,r2,r3} \n"
    //                       "stm %[pix], {r0,r1} \n"
    //                       "add %[pix], %[inc] \n"
    //                       "stm %[pix], {r2,r3} \n"
    //                       "add %[cnt], #1 \n"
    //                       "cmp %[cnt], #7 \n"
    //                       : [pix] "+r" (pix), [let] "+r" (let), [cnt] "+r" (cnt)
    //                       : [inc] "r" (inc)
    //                       : "memory", "r0", "r1", "r2", "r3", "cc");
    //     }
    //     asm volatile("ldm %[let], {r0,r1} \n"
    //                  "stm %[pix], {r0,r1}"
    //                  : [pix] "+r" (pix), [let] "+r" (let)
    //                  :
    //                  : "memory", "r0", "r1");

    // }
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
