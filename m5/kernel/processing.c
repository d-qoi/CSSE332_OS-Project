/*
  Milestone 3 kernel/interrupt.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */

#include "processing.h"
#include "lib/string.h"
#include "mem.h"
#include "vfs.h"
#include "fs/csse/csse.h"

int executeProgram(char *path, char * args, int shouldWait) {
  char buffer[CSSE_MAX_FSIZE];
  int argc;

  int i, f, bytesRead;
  int newProc, segment;

  f = fopen(path, 'r');
  bytesRead = fread(f, buffer, CSSE_MAX_FSIZE);
  fclose(f);
  
  if (bytesRead <= 0) {
    KDS
    println("Executable not found!");
    SDS
    return -1;
  }

  newProc = allocateProcess();
  KDS
  memcpySK(processTable[newProc].args, args, 256);
  println(processTable[newProc].args);
  SDS
  
  if (shouldWait) {
    KDS
    processTable[currentProcess].waiting = newProc;
    SDS
  }

  segment = TOSEGMENT(newProc + 2);
  for (i = 0; i < bytesRead; i++) {
    putInMemory(segment, i, buffer[i]);
  }

  initializeProgram(segment);
  return 0;
}

/* allocates a process if no segment is given */
int allocateProcess() {
  int i = 0;
  KDS
  for (i = 0; i < PROCESSLIMIT; i++) {
    if (processTable[i].running == 0) {
      processTable[i].running = 1;
      processTable[i].waiting = -1;
      processTable[i].sp = 0xFF00;
      SDS
      return i;
    }
  }
  SDS
  return -1;
}

/* get Running Segment */
int getRunningSegment() {
  int seg;
  KDS
  seg = TOSEGMENT(currentProcess + 2);
  SDS
  return seg;
}

void clearWait(int proc) {
  int i;
  KDS
  println("Clearing wait");
  for (i = 0; i < PROCESSLIMIT; i++) {
    if (processTable[i].waiting == proc) {
      processTable[i].waiting = -1;
    }
  }
  SDS
}

void terminate(int proc) {
  KDS
  println("terminating");
  printHex(proc);
  processTable[proc].running = 0;
  processTable[proc].sp = 0xFF00;
  processTable[proc].waiting = -1;
  /* clearWait(proc); */
  SDS
}

void handleTimerInterrupt(int segment, int sp) {
  int i;

  if (TOSEGMENT(currentProcess + 2) == segment){
    processTable[currentProcess].sp = sp;
  }
  if (processTable[currentProcess].running == 0) {
    clearWait(currentProcess);
  }

  for (i = 1; i <= PROCESSLIMIT; i++) {
    int pi = mod(currentProcess + i, PROCESSLIMIT);
    if (processTable[pi].running && processTable[pi].waiting == -1) {
      sp = processTable[pi].sp;
      segment = (pi + 2) * 0x1000;
      currentProcess = pi;
      break;
    }
  }
    
  returnFromTimer(segment, sp);
}
