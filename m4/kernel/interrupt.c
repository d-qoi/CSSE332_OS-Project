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
  int i, f, bytesRead;
  
  
  /* Read file and return if it failed. */
  f = fopen(path, 'r');
  bytesRead = fread(f, buffer, CSSE_MAX_FSIZE);
  fclose(f);
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

void copyLenOut(int len, char * src, char * tgt) {
  while (len > 0) {
    *tgt = getFromMemory(0x2000, src);
    src++;
    tgt++;
    len--;
  }
}

void copyLenIn(int len, char * src, char * tgt) {
  while (len > 0) {
    *src = putInMemory(0x2000, tgt);
    src++;
    tgt++;
    len--;
  }
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
  int f, bytesRead, len;
  char buffer[512];

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
      len = strlen((char *) bx);
      setKernelDataSegment();
      copyLenOut(len, (char *) bx, buffer);
      
      f = fopen(buffer, 'r');
      bytesRead = fread(f, buffer, CSSE_MAX_FSIZE);
      fclose(f);
      println(buffer);
      len = strlen((char *) buffer);
      copyLenIn(len, buffer, bx);
      
      restoreDataSegment();
    break;
    case 4: 
      /* Execute program at filename *bx in segment cx*/
      executeProgram((char *) bx, cx);
      break;
    case 5: /* Terminate current program. */
      terminate();
      break;
    case 6: /* Write a sector */
      writeSector((char *) bx, cx);
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
