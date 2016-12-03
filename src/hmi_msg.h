/*
 * hmi_msg.h
 *
 * Created: 02.12.2016 11:10:28
 *  Author: emaktse
 */

#ifndef _HMI_MSG_H_ /* Header Guard */
#define _HMI_MSG_H_ /* Header Guard */

#define VER_GCC "avr-gcc version: " __VERSION__
#define VER_FW "Version: %S built on: %S %S\n"
#define VER_LIBC "avr-libc version: %S"
#define PROMPT_FIRST_LETTER "Enter month's first letter >"


#define STUD_NAME "Maksim Tseljabov"

const char string_1[] PROGMEM = "January";
const char string_2[] PROGMEM = "February";
const char string_3[] PROGMEM = "March";
const char string_4[] PROGMEM = "April";
const char string_5[] PROGMEM = "May";
const char string_6[] PROGMEM = "June";

PGM_P const months[6] PROGMEM = 
{
    string_1,
    string_2,
    string_3,
    string_4,
    string_5,
    string_6
};

#endif /* Header Guard */