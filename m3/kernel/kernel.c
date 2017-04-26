/*
  Milestone 3 kernel/kernel.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */

#include "interrupt.h"
#include "fs.h"
#include "io.h"

int main() {
  char buffer[MAX_FSIZE];  /* this is the maximum size of a file */
  
  makeInterrupt21();
  
  printString("File read and print demo:\r\n\0");
  interrupt(0x21, 3, "/var/messag\0", buffer, 0);
  interrupt(0x21, 0, buffer, 0, 0);
  
  printString("File execute demo:\r\n\0");
  interrupt(0x21, 4, "/bin/tstpr2\0", 0x2000, 0);
  while(1);
}
