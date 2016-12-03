#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <string.h>
#include "uart.h"
#include "print_helper.h"
#include "hmi_msg.h"
#include "../lib/hd44780_111/hd44780.h"

#define BLINK_DELAY_MS 100

void main (void)
{
    /* Set pin 3 of PORTA for output */
    DDRA |= _BV(DDA3);
    /* Init in/out console in UART0 and print student name with new line at the end*/
    uart0_initialize();
    stdin = stdout = &uart0_io;
    fprintf_P(stdout, PSTR(STUD_NAME "\n"));
    /* Init error console as stderr in UART3 and print user code info */
    uart3_initialize();
    stderr = &uart3_out;
    fprintf_P(stderr, PSTR(VER_FW),
              PSTR(GIT_DESCR), PSTR(__DATE__), PSTR(__TIME__));
    fprintf_P(stderr, PSTR(VER_LIBC " " VER_GCC "\n"),
              PSTR(__AVR_LIBC_VERSION_STRING__));
    /* End UART3 init and info print */
    /*Initialize display and clear srceen*/
    lcd_init();
    lcd_clrscr();
    /*Print student name to LCD from program memory*/
    lcd_puts_P(PSTR(STUD_NAME));
    /* Print ASCII table */
    print_ascii_tbl(stdout);
    unsigned char charArray[128] = {0};

    for (unsigned char i = 0; i < sizeof(charArray); i++) {
        charArray[i] = i;
    }

    print_for_human(stdout, charArray, 128);

    while (1) {
        /* Set pin 3 high to turn LED on */
        PORTA |= _BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);
        /*Create char buffer.*/
        char letter = ' ';
        /*Prompt user to insert letter and output what was input*/
        fprintf_P(stdout, PSTR(PROMPT_FIRST_LETTER));
        fscanf(stdin, "%c", &letter);
        fprintf(stdout, "%c\n", letter);
        /* Clear second line of display. Then goto beginning of line on display*/
        lcd_clr(0X40, 16);
        lcd_goto(0x40);

        /*Compare the input char with first letter of records in months[] and in case of sucssess output month*/
        for (int i = 0; i < 6; i++) {
            if (!strncmp_P(&letter, (PGM_P)pgm_read_word(&months[i]), 1)) {
                fprintf_P(stdout, PSTR("%S\n"), (PGM_P)pgm_read_word(&months[i]));
                /*Output month(s) to display*/
                lcd_puts_P((PGM_P)pgm_read_word(&months[i]));
                lcd_putc(' '); //Put space between months, if more than 1 matches
            }
        }

        /* Set pin 3 low to turn LED off */
        PORTA &= ~_BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);
    }
}
