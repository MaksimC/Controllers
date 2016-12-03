/*
 * uart.h
 *
 * Created: 02.12.2016 0:45:18
 *  Author: emaktse
 */ 

#ifndef _UART_H_ /* Header Guard */
#define _UART_H_ /* Header Guard */

int uart0_putchar(char c, FILE *stream);
int uart0_getchar(FILE *stream);
int uart3_putchar(char c, FILE *stream);

void uart0_initialize(void);
void uart3_initialize(void);

/* http://www.ermicro.com/blog/?p=325 */

FILE uart0_io = FDEV_SETUP_STREAM(uart0_putchar, uart0_getchar, _FDEV_SETUP_RW);
FILE uart3_out = FDEV_SETUP_STREAM(uart3_putchar, NULL, _FDEV_SETUP_WRITE);

#endif /* Header Guard */
