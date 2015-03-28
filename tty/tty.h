#ifndef _TTY_H_
#define _TTY_H_

#include "types.h"
#include "mailbox.h"

#define PXWIDTH 1
#define BIT_DEPTH 8

#define CHAR_HEIGHT 15
#define CHAR_WIDTH 8
#define CHAR_PXWIDTH (PXWIDTH*CHAR_WIDTH)
#define CHAR_SIZE (CHAR_HEIGHT*CHAR_PXWIDTH)

#define BITMAP_SIDE 128
#define BITMAP_PXWIDTH BITMAP_SIDE*PXWIDTH
#define BITMAP_COLS (BITMAP_SIDE / CHAR_WIDTH)


#define INC_COL(con)                            \
    do {                                        \
        if (++con->col == con->last_col) {      \
            INC_ROW(con);                       \
        }                                       \
    } while (0)

#define INC_ROW(con)                            \
    do {                                        \
        con->col = 0;                           \
        if (++con->row == con->last_row)        \
            tty_scroll(con);                    \
    } while (0)

struct console {
    char *prompt;
    const u8 *letters;
    volatile u8 *framebuffer;
    int row,
        last_row,
        col,
        last_col;
    struct fb_info fbinfo __attribute__ ((aligned (16)));
};

void tty_init(struct console*);
void tty_write(struct console *, char);
void tty_write_str(struct console *, char*);
void tty_clear(struct console *);


#endif
