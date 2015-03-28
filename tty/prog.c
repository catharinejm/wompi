#include "tty.h"
#include "periph.h"
#include "types.h"

int wombat (void) {
    struct console con;
    
    uart_init();
    timer_init();
    tty_init(&con);

    for (char c = '!'; c <= '~'; c++)
        tty_write(&con, c);
    tty_write(&con, '\n');
    while (1) {
        u32 c = uart_recv();
        tty_write(&con, c);
    }
    
    return 0;
}
