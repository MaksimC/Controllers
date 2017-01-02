#ifndef _HMI_MSG_H_ /* Header Guard */
#define _HMI_MSG_H_ /* Header Guard */
#define VER_GCC "avr-gcc version: " __VERSION__
#define VER_FW "Version: %S built on: %S %S\n"
#define VER_LIBC "avr-libc version: %S"
#define PROMPT_FIRST_LETTER "Enter month's first letter >"
#define LBL_UPTIME "Uptime"
#define STUD_NAME "Maksim Tšeljabov"
#define CLI_START_MSG "Use backspace to delete entry and enter to confirm.\nArrow keys and del do not work currently.\nUse Help command to learn about commands"
#define HELP_CMD "help"
#define HELP_HELP "Get help"
#define VER_CMD "version"
#define VER_HELP "Print FW version"
#define ASCII_CMD "ascii"
#define ASCII_HELP "print ASCII tables"
#define MONTH_CMD "month"
#define MONTH_HELP "Find matching month from lookup list. Usage: month <string>"
#define CLI_HELP_MSG "Implemented commands:"
#define CLI_NO_CMD "Command not implemented.\n Use <help> to get help."
#define CLI_ARGS_MSG "To few or to many arguments for this command\nUse <help>"

extern PGM_P const months[];
extern const char help_cmd[];
extern const char help_help[];
extern const char ver_cmd[];
extern const char ver_help[];
extern const char ascii_cmd[];
extern const char ascii_help[];
extern const char month_cmd[];
extern const char month_help[];

#endif /* Header Guard */
