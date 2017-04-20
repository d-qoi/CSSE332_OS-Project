#include "math.h"

#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

/*
Definitions of interrupt commands
*/

#define BIOS_PRINT_CHAR 0x10
#define BIOS_READ_CHAR 0x16
#define BIOS_READ_SECT 0x13

#define PRINT_COMMAND 0xE

#define BACKSPACE 0x8
#define ENTER 0xD
#define C_RET '\r'
#define LINE_FEED 0xA
#define NULL_TERM 0x0

#define MOVE_H(val) val*256

#define PRINT_CHAR(character)                                           \
  interrupt(BIOS_PRINT_CHAR,                                            \
            MOVE_H(PRINT_COMMAND) + character,                          \
            0, 0, 0)


#define GET_CHAR interrupt(BIOS_PRINT_CHAR, 0, 0, 0, 0)

#define READ_SECTOR(data, sector)                                       \
  interrupt(BIOS_READ_SECT, data,                                       \
            MOVE_H(mod(sector,36)) + mod(sector, 18) + 1,               \
            MOVE_H(mod(mod(sector, 18), 2)) + 0)

void printString(char * string);

void readString(char * buffer);

#endif

