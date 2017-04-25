/*
  Milestone 3 kernel.c file
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */

#include "interrupt.h"

#define TEXT_COLOR 0x7
#define VIDEO_BASE_ADDR 0xB000
#define SCREEN_BASE_ADDR 0x8000
#define SCREEN_LOCATION_CHAR(line, character)\
  SCREEN_BASE_ADDR + ((line - 1) * 80 + character) * 2
#define SCREEN_LOCATION_COLOR(line, character)\
  SCREEN_BASE_ADDR + ((line - 1) * 80 + character) * 2 + 1

int main() {
  char buffer[13312];  /* this is the maximum size of a file */

  printString("Hello World\r\n\0");
  
  makeInterrupt21();
  /* read the file into buffer */
  interrupt(0x21, 3, "messag\0", buffer, 0);  
  printString("Step 2!\r\n\0");
  interrupt(0x21, 0, buffer, 0, 0);	    /* print out the file */
  while(1);	/* hang */
}
