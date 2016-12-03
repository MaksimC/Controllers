/*
 * main.c
 *
 * Created: 02.12.2016 0:45:18
 *  Author: emaktse
 */

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "print_helper.h"
#include "hmi_msg.h"
#include <string.h>

#define BLINK_DELAY_MS 100

void main (void)
{
    /* Set pin 3 of PORTA for output */
    DDRA |= _BV(DDA3);
    /* Init in/out console in UART0 and print student name with new line at the end*/
    uart0_initialize();
    stdin = stdout = &uart0_io;
    fprintf(stdout, STUD_NAME "\n");
    /* END of init in/out console in UART0 and print student name with new line at the end*/
    /* Init error console as stderr in UART3 and print user code info */
    uart3_initialize();
    stderr = &uart3_out;
    fprintf(stderr, "Version: %s built on: %s %s\n",
            GIT_DESCR, __DATE__, __TIME__);
    fprintf(stderr, "avr-libc version: %s\n", __AVR_LIBC_VERSION_STRING__);
    /* End UART3 init and info print */
    /* Print ASCII HEX to console via UART3 */
    print_ascii_tbl(stdout);
    unsigned char charArray[128] = {0};

    for (unsigned char i = 0; i < 128; i++) {
        charArray[i] = i;
    }

    print_for_human(stdout, charArray, 128);
    /* END of print ASCII HEX values via UART3 */

    while (1) {
        /* Set pin 3 high to turn LED on */
        PORTA |= _BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);
        /*Create char buffer.*/
        char letter = ' ';
        /*Prompt user to insert letter and output what was input*/
        fprintf(stdout, "Enter month's first letter >");
        fscanf(stdin, "%c", &letter);
        fprintf(stdout, "%c\n", letter);

        /*Check the input char with first letter of records in months[] and in case of sucssess output month*/
        for (int i = 0; i < 6; i++) {
            if (!strncmp(&letter, months[i], 1)) {
                fprintf(stdout, "%s\n", months[i]);
            }
        }

        /* Set pin 3 low to turn LED off */
        PORTA &= ~_BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);
    }
}
