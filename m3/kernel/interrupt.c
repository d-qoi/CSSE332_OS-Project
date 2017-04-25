/*
  Milestone 3 kernel/interrupt.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */
 
#include "io.h"
#include "interrupt.h"

void handleInterrupt21(int ax, int bx, int cx, int dx) {
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
      readFile((char *) bx, (char *) cx);
      break;
  }
}
