/*
  Milestone 3 kernel/interrupt.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */

#include "processing.h"
#include "lib/string.h"
#include "vfs.h"
#include "fs/csse/csse.h"


int executeProgram(char *path, int shouldWait) {
  char buffer[CSSE_MAX_FSIZE];
  int i, f, bytesRead;
  int newProc;

  newProc = allocateProcess();
  if (shouldWait) {
    processTable[currentProcess].waiting = newProc;
  }


  segment = TOSEGMENT(newProc + 2);

  /* Read file and return if it failed. */
  f = fopen(path, 'r');
  bytesRead = fread(f, buffer, CSSE_MAX_FSIZE);
  fclose(f);
  if (!bytesRead)
    return -1;

  for (i = 0; i < bytesRead; i++) {
    putInMemory(segment, i, buffer[i]);
  }

  initializeProgram(segment);
  return 0;
}

/* allocates a process if no segment is given */
int allocateProcess() {
  int i = 0;
  println("allocation");
  for (i = 0; i < PROCESSLIMIT; i++) {
    if (processTable[i].running == 0) {
      processTable[i].running = 1;
      return i;
    }
  }
  return -1;
}

/* get Running Segment */
int getRunningSegment() {
  return TOSEGMENT(currentProcess + 2);
}

void clearWait(int proc) {
  int i;
  for (i = 0; i < PROCESSLIMIT; i++) {
    if (processTable[i].waiting == proc) {
      processTable[i].waiting = -1;
    }
  }
}

void terminate(int proc) {
  processTable[proc].running = 0;
  processTable[proc].sp = 0xFF00;
  processTable[proc].waiting = -1;
  clearWait(proc);
}

void handleTimerInterrupt(int segment, int sp) {
  int i, temp, temp2;
  if (TOSEGMENT(currentProcess + 2) == segment)
    processTable[currentProcess].sp = sp;
  if (processTable[currentProcess].running == 0) {
    clearWait(currentProcess);
  }
  for (i = currentProcess + 1; i < PROCESSLIMIT; i++) {
    if (processTable[i].running && processTable[i].waiting == -1) {
      /* println("Dispatch"); */
      sp = processTable[i].sp;
      segment = (i + 2) * 0x1000;
      currentProcess = i;
      returnFromTimer(segment, sp);
    }
  }
  for (i = 0; i < currentProcess + 1; i++) {
    if (processTable[i].running  && processTable[i].waiting == -1) {
      /* println("Dispatch"); */
      sp = processTable[i].sp;
      segment = (i + 2) * 0x1000;
      currentProcess = i;
      returnFromTimer(segment, sp);
    }
  }
  returnFromTimer(segment, sp);
}
