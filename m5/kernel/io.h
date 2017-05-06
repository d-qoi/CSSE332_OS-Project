/*
  Milestone 3 kernel/io.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */
 
#include "lib/math.h"
 
#ifndef _IO_H_
#define _IO_H_

#define BIOS_PRINT_CHAR 0x10
#define BIOS_READ_CHAR 0x16
#define BIOS_DISK_INTERACT 0x13

#define DISK_READ 2
#define DISK_WRITE 3

#define PRINT_COMMAND 0xE

#define BACKSPACE 0x8
#define ENTER 0xD
#define C_RET '\r'
#define LINE_FEED 0xA
#define NULL_TERM 0x0

#define MOVE_H(val) val*256

#define PRINT_CHAR(character)						\
  interrupt(BIOS_PRINT_CHAR,                                          \
            MOVE_H(PRINT_COMMAND) + character,                        \
            0, 0, 0)


#define GET_CHAR interrupt(BIOS_READ_CHAR, 0, 0, 0, 0)
            
void printHex(int value);

void printString(char * string);

void println(char * string);

void readString(char * buffer);

void readSector(char * buffer, int sector);
void readSectorFrom(char * buffer, int disk, int sector);
void writeSectorTo(char * buffer, int disk, int sector);

#endif
