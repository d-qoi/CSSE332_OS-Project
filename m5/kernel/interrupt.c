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
  int f, bytesRead, len, num;
  char buffer[1024];
  char buffer2[257];

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
    /* Execute program at filename *bx, args *cx in segment dx*/
    len = strlen((char *) bx);

    setKernelDataSegment();
    copyLenOut(len, (char *) bx, buffer);

    executeProgram(buffer, cx);

    restoreDataSegment();
    break;
  case 5: /* Terminate current program. */
    setKernelDataSegment();
    terminate(currentProcess);
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
  case 10:
    setKernelDataSegment();
    terminate(bx);
    restoreDataSegment();
    break;
  case 11:
    setKernelDataSegment();
    terminate(currentProcess);
    restoreDataSegment();
    break;
  }
}

