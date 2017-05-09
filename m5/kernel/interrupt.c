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

void copyLenOut(int len, char *src, char *tgt) {
  while (len > 0) {
    *tgt = getFromMemory(GRS, src); /* GRS is a macro defined in processing.h, for Get Running Segment */
    src++;
    tgt++;
    len--;
  }
}

void copyLenIn(int len, char *src, char *tgt) {
  while (len > 0) {
    putInMemory(GRS, tgt, *src);
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
    copyLenOut(len, (char *) bx, buffer);
    buffer[len] = 0;

    f = fopen(buffer, 'r');
    if (f < 0) {
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
    copyLenIn(len, buffer, cx);

    restoreDataSegment();
    break;
  case 4:
    /* Execute program at filename *bx in segment cx*/
    len = strlen((char *) bx);
    setKernelDataSegment();

    copyLenOut(len, (char *) bx, buffer);

    executeProgram(buffer, cx);

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
    copyLenOut(len, (char *) bx, buffer);
    buffer[len] = 0;
    fdel(buffer);

    restoreDataSegment();
    break;
  case 8: /* Write a file */
    len = strlen((char *) bx);
    setKernelDataSegment();
    copyLenOut(len, (char *) bx, buffer);
    buffer[len] = 0;
    f = fopen(buffer, 'w');

    restoreDataSegment();
    len = strlen((char *) cx);
    setKernelDataSegment();
    memset(buffer, 0, 512);
    copyLenOut(len, (char *) cx, buffer);
    fwrite(f, buffer, len + 1);
    fclose(f);

    restoreDataSegment();
    break;
  case 9:
    len = strlen((char *) bx);
    setKernelDataSegment();
    copyLenOut(len, (char *) bx, buffer);
    buffer[len] = 0;

    freaddir(buffer, buffer);

    len = strlen((char *) buffer);
    copyLenIn(1024, buffer, cx);

    restoreDataSegment();
    break;
  }
}

void handleTimerInterrupt(int segment, int sp) {
  int i, temp, temp2;
  temp = sp;
  temp += 1;
  /* println("Tic"); */
  /* printHex(1etDataSegment()); */
  /* println("\0"); */
  /* printHex(getCodeSegment()); */
  /* println("\0"); */
  /* printHex(getStackSegment()); */
  /* println("\0"); */
  /* printHex(getEsegment()); */
  /* println("\0"); */
  /* printHex(segment); */
  /* println("\0"); */
  /* printHex(temp); */
  /* println("\0"); */
  if (segment == processTable[currentProcess].segment)
    processTable[currentProcess].sp = sp;
  for (i = currentProcess + 1; i < PROCESSLIMIT; i++) {
    if (processTable[i].running) {
      /* println("Dispatch"); */
      sp = processTable[i].sp;
      segment = (i + 2) * 0x1000;
      currentProcess = i;
      returnFromTimer(segment, sp);
    }
  }
  for (i = 0; i < currentProcess + 1; i++) {
    if (processTable[i].running) {
      /* println("Dispatch"); */
      sp = processTable[i].sp;
      segment = (i + 2) * 0x1000;
      currentProcess = i;
      returnFromTimer(segment, sp);
    }
  }
  returnFromTimer(segment, sp);
}
