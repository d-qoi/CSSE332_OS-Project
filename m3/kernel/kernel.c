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
  char exShell[11];
  char file[12];
  
  exShell[0] = '/';
  exShell[1] = 'b';
  exShell[2] = 'i';
  exShell[3] = 'n';
  exShell[4] = '/';
  exShell[5] = 's';
  exShell[6] = 'h';
  exShell[7] = 'e';
  exShell[8] = 'l';
  exShell[9] = 'l';
  exShell[10] = '\0';

  file[0] = '/';
  file[1] = 'v';
  file[2] = 'a';
  file[3] = 'r';
  file[4] = '/';
  file[5] = 'm';
  file[6] = 'e';
  file[7] = 's';
  file[8] = 's';
  file[9] = 'a';
  file[10] = 'g';
  file[11] = '\0';
  
  makeInterrupt21();
  
  printString("File read and print demo:\r\n\0");
  interrupt(0x21, 3, file, buffer, 0);
  interrupt(0x21, 0, buffer, 0, 0);
  
  printString("Starting shell:\r\n\0");
  interrupt(0x21, 4, exShell, 0x2000, 0);
  while(1);
}
