/*
  Milestone 3 kernel/io.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */

#include "io.h"


void printHex(int value) {
  if (value > 16) {
    int newVal = div(value, 16);
    printHex(newVal);
    value -= newVal*16;
  }
  if (value > 10) {
    PRINT_CHAR((value-10) + 'A');
  }
  else if (value < 10) {
    PRINT_CHAR(value + '0');
  }
}


void printString(char * string) {
  int ind = 0;
  char next = string[ind];
  while (next != '\0') {
    PRINT_CHAR(next);
    ind++;
    next = string[ind];
  }
}

void println(char * string) {
  char lineEnd[3];
  lineEnd[0] = '\n';
  lineEnd[1] = '\r';
  lineEnd[2] = '\0';
  printString(string);
  printString(lineEnd);
}

void readString(char * buffer) {
  int ind = 0;
  char temp;
  while(1) {
    if (ind >= 76)
      break;
    temp = GET_CHAR;
    if (temp == BACKSPACE) {
      if (ind > 0) {
	ind--;
	PRINT_CHAR(BACKSPACE);
	PRINT_CHAR(' ');
	PRINT_CHAR(BACKSPACE);
      }
    }else {

      if (temp == ENTER) {
	PRINT_CHAR(C_RET);
	PRINT_CHAR(LINE_FEED);
	break;
      }
      PRINT_CHAR(temp);
      buffer[ind] = temp;
      ind++;
    }
  }
  buffer[ind++] = C_RET;
  buffer[ind++] = LINE_FEED;
  buffer[ind] = NULL_TERM;
  return;
}

void readSector(char * buffer, int sector) {
  int relSector, head, track;
  
  relSector = mod(sector, 18) + 1;
  
  head = mod(div(sector, 18), 2);
  
  track = div(sector, 36);
  
  interrupt(BIOS_DISK_INTERACT, MOVE_H(DISK_READ) + 1, buffer,        \
          MOVE_H(track) + relSector,             \
          MOVE_H(head) + 0);
}
