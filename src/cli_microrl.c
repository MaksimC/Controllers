#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "hmi_msg.h"
#include "print_helper.h"
#include "cli_microrl.h"

typedef struct cli_cmd {
    PGM_P cmd;
    PGM_P help;
    void (*func_p)();
    const uint8_t func_argc;
} cli_cmd_t;


const cli_cmd_t cli_cmds[] = {
    {help_cmd, help_help, cli_print_help, 0},
    {ver_cmd, ver_help, cli_print_ver, 0},
    {ascii_cmd, ascii_help, cli_print_ascii_tbls, 0},
    {month_cmd, month_help, cli_handle_month, 1}
};


void cli_print(const char *str)
{
    printf("%s", str);
}


char cli_get_char(void)
{
    if (uart0_peek() != UART_NO_DATA) {
        return uart0_getc() & UART_STATUS_MASK;
    } else {
        return 0x00;
    }
}


void cli_print_help(const char *const *argv)
{
    (void) argv;
    printf_P(PSTR("\n"));
    printf_P(PSTR(CLI_HELP_MSG "\n"));

    for (uint8_t i = 0; i < NUM_ELEMS(cli_cmds); i++) {
        printf_P(cli_cmds[i].cmd);
        printf_P(PSTR(" : "));
        printf_P(cli_cmds[i].help);
        printf_P(PSTR("\n"));
    }
}


void print_version(FILE *stream)
{
    //Print version to UART3 console
    fprintf_P(stream, PSTR(VER_FW),
              PSTR(GIT_DESCR), PSTR(__DATE__), PSTR(__TIME__));
    fprintf_P(stream, PSTR(VER_LIBC " " VER_GCC "\n"),
              PSTR(__AVR_LIBC_VERSION_STRING__));
}


void cli_print_ver(const char *const *argv)
{
    (void) argv;
    printf_P(PSTR("\n"));
    print_version (stdout);
}


void cli_print_ascii_tbls(const char *const *argv)
{
    (void) argv;
    putc('\n', stdout);
    //Print ASCII table
    print_ascii_tbl(stdout);
    unsigned char charArray[128];

    for (unsigned char i = 0; i < sizeof(charArray); i++) {
        charArray[i] = i;
    }

    print_for_human(stdout, charArray, 128);
}


void cli_handle_month(const char *const *argv)
{
    putc('\n', stdout);
    // Clear second line of display. Then goto beginning of line on display
    lcd_clr(0X40, 16);
    lcd_goto(0x40);

    // Compare the input char with first letter of records in months[] and in case of success output month
    for (int i = 0; i < 6; i++) {
        if (!strncmp_P(argv[1], (PGM_P)pgm_read_word(&months[i]), strlen(argv[1]))) {
            fprintf_P(stdout, PSTR("%S\n"), (PGM_P)pgm_read_word(&months[i]));
            // Output month(s) to display
            lcd_puts_P((PGM_P)pgm_read_word(&months[i]));
            lcd_putc(' '); //Put space between months, if more than 1 matches
        }
    }
}


void cli_print_cmd_error(void)
{
    printf_P(PSTR("\n"));
    printf_P(PSTR(CLI_NO_CMD "\n"));
}


void cli_print_cmd_arg_error(void)
{
    printf_P(PSTR("\n"));
    printf_P(PSTR(CLI_ARGS_MSG "\n"));
}


int cli_execute(int argc, const char *const *argv)
{
    for (uint8_t i = 0; i < NUM_ELEMS(cli_cmds); i++) {
        if (!strcmp_P(argv[0], cli_cmds[i].cmd)) {
            // Test do we have correct arguments to run command
            // Function arguments count shall be defined in struct
            if ((argc - 1) != cli_cmds[i].func_argc) {
                cli_print_cmd_arg_error();
                return 0;
            }

            // Hand argv over to function pointer,
            // cross fingers and hope that funcion handles it properly
            cli_cmds[i].func_p (argv);
            return 0;
        }
    }

    cli_print_cmd_error();
    return 0;
}
