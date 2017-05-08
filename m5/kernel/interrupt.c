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
#include "processing.h"

void terminate() {
  processTable[currentProcess].running = 0;
}

void copyBytesFromUserspace(int len, char * src, char * tgt) {
  while (len > 0) {
    *tgt = getFromMemory(0x2000, src);
    src++;
    tgt++;
    len--;
  }
}

void copyBytesToUserspace(int len, char * src, char * tgt) {
  while (len > 0) {
    putInMemory(0x2000, tgt, *src);
    src++;
    tgt++;
    len--;
  }
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
  int f, bytesRead, len;
  char buffer[1024];

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
      copyBytesFromUserspace(len, (char *) bx, buffer);
      buffer[len] = 0;

      f = fopen(buffer, 'r');
      if (f < 0){
        println("Could not open file.");
        restoreDataSegment();
        break;
      }
      bytesRead = fread(f, buffer, CSSE_MAX_FSIZE);
      fclose(f);
      
      if (bytesRead < 0) {
        restoreDataSegment();
        break;
      }
      
      len = strlen((char *) buffer);
      copyBytesToUserspace(len, buffer, cx);
      
      restoreDataSegment();
    break;
    case 4: 
      /* Execute program at filename *bx in segment cx*/
      len = strlen((char *) bx);
      setKernelDataSegment();
      
      copyBytesFromUserspace(len, (char *) bx, buffer);
      
      executeProgram(buffer);
      
      restoreDataSegment();
      break;
    case 5: /* Terminate current program. */
      setKernelDataSegment();
      terminate();
      break;
    case 6: /* Write a sector */
      writeSector((char *) bx, (char *) cx);
      break;
    case 7: /* Delete a file */
      len = strlen((char *) bx);
      setKernelDataSegment();
      copyBytesFromUserspace(len, (char *) bx, buffer);
      buffer[len] = 0;
      fdel(buffer);
      
      restoreDataSegment();
      break;
    case 8: /* Write a file */
      len = strlen((char *) bx);
      setKernelDataSegment();
      copyBytesFromUserspace(len, (char *) bx, buffer);
      buffer[len] = 0;
      f = fopen(buffer, 'w');
      
      restoreDataSegment();
      len = strlen((char *) cx);
      setKernelDataSegment();
      memset(buffer, 0, 512);
      copyBytesToUserspace(len, (char *) cx, buffer);
      fwrite(f, buffer, len+1);
      fclose(f);
      
      restoreDataSegment();
      break;
    case 9:
      len = strlen((char *) bx);
      setKernelDataSegment();
      copyBytesFromUserspace(len, (char *) bx, buffer);
      buffer[len] = 0;
      
      freaddir(buffer, buffer);
      
      len = strlen((char *) buffer);
      copyBytesToUserspace(1024, buffer, cx);
      
      restoreDataSegment();
      break;
  }
}

void handleTimerInterrupt(int segment, int sp) {
  char msg[10];
  int i, temp, temp2;
  memcpy(msg, "findme!", 8);
  i=78;
  println("\nTic");
  printSegments();
  printHex(sp);
  println("Tock");
  printHex(currentProcess);
  println("\0");
  while(1);
  processTable[currentProcess].sp = sp;
  for (i = 0; i < PROCESSLIMIT; i++) {
    if (processTable[i].running) {
      println("Dispatch");
      printHex(processTable[i].sp);
      sp = processTable[i].sp;
      segment = (i + 2) * 0x1000;
      currentProcess = i;
      break;
    }
  }
  println("\0");
  printHex(currentProcess);
  printHex(segment);
  printHex(sp);
  println("\0");
  returnFromTimer(segment, sp);
}
