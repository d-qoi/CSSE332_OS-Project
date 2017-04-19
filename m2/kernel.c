/*
  Milestone 2 kernel.c file
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

char string[12] = "Hello World";

int main() {
  int i;
  for (i = 0; i < 11; i++) {
    putInMemory(VIDEO_BASE_ADDR, SCREEN_LOCATION_CHAR(1, i), string[i]);
    putInMemory(VIDEO_BASE_ADDR, SCREEN_LOCATION_COLOR(1, i), TEXT_COLOR);
    
  }
  printString("Hello World\0");
  
  /* Done, so hang */
  while(1);
  return 0;
}
