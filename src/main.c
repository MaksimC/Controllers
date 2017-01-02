#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "uart-wrapper.h"
#include "print_helper.h"
#include "hmi_msg.h"
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "cli_microrl.h"
#include "../lib/helius_microrl/microrl.h"

#define BAUD 9600



volatile uint32_t system_time;

// Create microrl object and pointer on it
microrl_t rl;
microrl_t *prl = &rl;


static inline void init_system_clock(void)
{
    TCCR5A = 0; // Clear control register A
    TCCR5B = 0; // Clear control register B
    TCCR5B |= _BV(WGM52) | _BV(CS52); // CTC and fCPU/256
    OCR5A = 62549; // 1 s
    TIMSK5 |= _BV(OCIE5A); // Output Compare A match Interrupt Enable
}


static inline void initialize_hardware()
{
    /* Set pin 3 of PORTA for output */
    DDRA |= _BV(DDA3);
    /* Init in/out console in UART0*/
    uart0_init(UART_BAUD_SELECT(BAUD, F_CPU));
    stdin = stdout = &uart0_io;
    /* Init error console in UART3*/
    uart3_init(UART_BAUD_SELECT(BAUD, F_CPU));
    stderr = &uart3_out;
    /* Initialize counter */
    init_system_clock();
    /* System Enable Interrupts */
    sei();
    /* Initialize display and clear srceen */
    lcd_init();
    lcd_clrscr();
}


static inline void print_console (void)
{
    print_version(stderr);
    // print student name
    fprintf_P(stdout, PSTR(STUD_NAME));
    fputc('\n', stdout); // Add a new line to the uart printout
    fprintf_P(stdout, PSTR(CLI_START_MSG));
    lcd_puts_P(PSTR(STUD_NAME));
    fputc('\n', stdout); // Add a new line to the uart printout
}


static inline void start_cli(void)
{
    // Call init with ptr to microrl instance and print callback
    microrl_init (prl, cli_print);
    // Set callback for execute
    microrl_set_execute_callback (prl, cli_execute);
}


static inline void heartbeat ()
{
    static uint32_t current_time;
    uint32_t system_time_copy;
    /*Copy of system_time using atomic block*/
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        system_time_copy = system_time;
    }

    if (current_time != system_time_copy) {
        /* Toggle led */
        PORTA ^= _BV(PORTA3);
        /* Printout system uptime */
        fprintf_P(stderr, PSTR(LBL_UPTIME ": %lu s\n"), system_time_copy);
    }

    current_time = system_time_copy;
}


void main (void)
{
    initialize_hardware();
    print_console();
    start_cli();

    while (1) {
        heartbeat();
        // CLI commands are handled in cli_execute()
        microrl_insert_char(prl, cli_get_char());
        //month_check();
    }
}


/* System clock ISR */
ISR(TIMER5_COMPA_vect)
{
    system_time++;
}
