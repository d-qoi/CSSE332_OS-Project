#include "interrupt.h"

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
