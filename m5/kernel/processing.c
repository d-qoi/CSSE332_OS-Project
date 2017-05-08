/*
  Milestone 3 kernel/interrupt.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */

#include "processing.h"
#include "lib/string.h"
#include "vfs.h"
#include "fs/csse/csse.h"


int executeProgram(char *path) {
  char buffer[CSSE_MAX_FSIZE];
  int i, f, bytesRead;
  struct process *currentProcess;

  currentProcess = allocateProcess();
  
  /* Read file and return if it failed. */
  f = fopen(path, 'r');
  bytesRead = fread(f, buffer, CSSE_MAX_FSIZE);
  fclose(f);
  if (!bytesRead)
    return -1;

  for (i = 0; i < bytesRead; i++) {
    putInMemory(currentProcess->segment, i, buffer[i]);
  }

  initializeProgram(currentProcess->segment);
  printString("Initialized ");
  println(path);
  return 0;
}

/* allocates a process if no segment is given */
struct process *allocateProcess() {
  int i = 0;
  for (i = 0; i < PROCESSLIMIT; i++) {
    if (processTable[i].segment == 0) {
      processTable[i].segment = TOSEGMENT(i + 2);
      processTable[i].running = 1;
      return processTable + i;
    }
  }
  return 0;
}

/*
returns the process struct that works with the segment
or returns a new process struct
 */
struct process *reallocateProcess(int segment) {
  segment = TOINT(segment);
  if (((segment - 2) >= 0) && ((segment - 2) <= 32)) {
    processTable[segment - 2].segment = TOSEGMENT(segment);
    return &processTable[segment - 2];
  }
  return allocateProcess();
}

/* zeroes out a process struct at a given segment */
struct process *freeProcess(int segment) {
  segment = TOINT(segment);
  if (((segment - 2) >= 0) && ((segment - 2) <= 32)) {
    memset(processTable[segment - 2], 0, sizeof(struct process));
    return &processTable[segment - 2];
  }
  return 0;
}

struct process *getProcessByName(char *name) {
  int i;
  for (i = 0; i < PROCESSLIMIT; i++)
    if (strcmp(name, processTable[i].name))
      return &processTable[i];
  return 0;
}

struct process *getCurrentProcess() {
  int i;
  for (i = 0; i < PROCESSLIMIT; i++)
    if (processTable[i].running)
      return &processTable[i];

  /* if it is unable to find a running process */
  processTable[0].running = 1;
  return &processTable[0];
}

/* get Running Segment */
int getRunningSegment() {
	int i;
  for (i = 0; i < PROCESSLIMIT; i++)
    if (processTable[i].running)
      return processTable[i].segment;
}

