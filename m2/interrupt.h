#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#define PRINT_COMMAND 0xE

#define BACKSPACE 0x8
#define NEWLINE 0xD
#define LINE_FEED 0xA
#define NULL_TERM 0x0

#define PRINT_CHAR(character) interrupt(0x10, PRINT_COMMAND * 256 + character, 0, 0, 0)
#define GET_CHAR interrupt(0x16, 0, 0, 0, 0)

void printString(char * string);

void readString(char * buffer);

#endif

