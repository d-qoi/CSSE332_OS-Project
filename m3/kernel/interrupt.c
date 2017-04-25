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
  bytesRead = readFile(name, buffer);
  if (!bytesRead) 
    return 1;
  
  for (i = 0; i < bytesRead; i++) {
    putInMemory(segment, i, buffer[i]);
  }
  
  launchProgram(segment);
  return 0;
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
  int ret;
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
      ret = readFile((char *) bx, (char *) cx);
      if (!ret) {
        char msg[31];
        msg[0] = 'r'; msg[1] = 'e'; msg[2] = 'a'; msg[3] = 'd';
        msg[4] = 'F'; msg[5] = 'i'; msg[6] = 'l'; msg[7] = 'e';
        msg[8] = ' '; msg[9] = 'r'; msg[10] = 'e'; msg[11] = 'a';
        msg[12] = 'd'; msg[13] = ' '; msg[14] = '0'; msg[15] = ' ';
        msg[16] = 'b'; msg[17] = 'y'; msg[18] = 't'; msg[19] = 'e';
        msg[20] = 's'; msg[21] = ' '; msg[22] = 'i'; msg[23] = 'n';
        msg[24] = ' '; msg[25] = '\0';
        strcat(msg, bx);
        println(msg);
      }
      break;
    case 4:
      executeProgram((char *) bx, cx);
      break;
    case 5:
      while(1); /* Hang for now */
      break;
  }
}
