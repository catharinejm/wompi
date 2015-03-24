#include "tty.h"
#include "periph.h"
#include "types.h"

int wombat (void) {
    struct console con;
    
    uart_init();
    timer_init();
    tty_init(&con);
    return 0;
}
