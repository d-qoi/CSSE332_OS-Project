/*
  Milestone 3 kernel/interrupt.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */
 
#include "io.h"
#include "fs.h"
#include "interrupt.h"
#include "lib/string.h"

/*TODO: Move this elsewhere when we have a good spot for it.*/
int executeProgram(char * name, int segment) {
  char buffer[MAX_FSIZE];
  int i, bytesRead;
  
  /* Read file and return if it failed. */
  bytesRead = readFileFromPath(name, buffer);
  if (!bytesRead) 
    return 1;
  
  for (i = 0; i < bytesRead; i++) {
    putInMemory(segment, i, buffer[i]);
  }
  
  launchProgram(segment);
  return 0;
}

void terminate() {
  char exShell[11];
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

  interrupt(0x21, 4, exShell, 0x2000, 0);
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
  int ret;
  switch (ax) {
    case 0: /* Print *bx as a string */
      printString((char *) bx);
      break;
    case 1: /* Read string to *bx */
      readString((char *) bx);
      break;
    case 2: /* Read sector cx to bx */
      readSector((char *) bx, cx);
      break;
    case 3: /* Read file at *bx into *cx */
      ret = readFileFromPath((char *) bx, (char *) cx);
      if (!ret) {
        char * msg = "ERROR: readFile read 0 bytes from ";
        strcat(msg, bx);
        println(msg);
      }
      break;
    case 4: 
      /* Execute program at filename *bx in segment cx*/
      executeProgram((char *) bx, cx);
      break;
    case 5: /* Terminate current program. */
      terminate();
      break;
    case 6:
      listFilesInDir((char *) bx, (char *) cx);
      break;
  }
}
