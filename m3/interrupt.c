/*
  Milestone 2 interrupt.c file
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */
#include "interrupt.h"
#include "fs.h"

void printString(char * string) {
  int ind = 0;
  char next = string[ind];
  while (next != '\0') {
    PRINT_CHAR(next);
    ind++;
    next = string[ind];
  }
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
  READ_SECTOR(buffer, sector);
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
  switch (ax) {
    case 0:
      printString((char *) bx);
      break;
    case 1:
      readString((char *) bx);
      break;
    case 2:
      readSector((char *) bx, cx);
      break;
    case 3:
      readFile((char *) bx, (char *) cx);
      break;
  }
}
