/*
  Milestone 3 kernel/interrupt.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */
 
#include "io.h"
#include "vfs.h"
#include "fs/csse/csse.h"
#include "interrupt.h"
#include "lib/string.h"

/*TODO: Move this elsewhere when we have a good spot for it.*/
int executeProgram(char * path, int segment) {
  char buffer[CSSE_MAX_FSIZE];
  int i, bytesRead;
  
  /* Read file and return if it failed. */
  bytesRead = fread(path, buffer, CSSE_MAX_FSIZE);
  if (!bytesRead) 
    return -1;
  
  for (i = 0; i < bytesRead; i++) {
    putInMemory(segment, i, buffer[i]);
  }
  
  launchProgram(segment);
  return 0;
}

void terminate() {
  interrupt(0x21, 4, "/bin/shell", 0x2000, 0);
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
  int f, bytesRead;
      
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
      f = fopen((char *) bx, 'r');
      bytesRead = fread(f, (char *) cx, CSSE_MAX_FSIZE);
      fclose(f);

      if (bytesRead == 0) {
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
    case 6: /* Write a sector */
      writeSector((char *)bx, cx);
      break;
    case 7: /* Delete a file */
      break;
    case 8: /* Write a file */
      break;
    case 9:
      freaddir((char *) bx, (char *) cx);
      break;
  }
}

void handleTimerInterrupt(int segment, int sp){
  returnFromTimer(segment, sp);
}
