#ifndef _PERIPH_H_
#define _PERIPH_H_

void uart_init ( void );
unsigned int uart_lcr ( void );
void uart_flush ( void );
void uart_send ( unsigned int );
unsigned int uart_recv ( void );
unsigned int uart_check ( void );
void hexstring ( unsigned int );
void hexstrings ( unsigned int );
void timer_init ( void );
unsigned int timer_tick ( void );

#endif
