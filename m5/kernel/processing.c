/*
  Milestone 3 kernel/interrupt.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */

#include "processing.h"
#include "lib/string.h"
#include "vfs.h"
#include "fs/csse/csse.h"
#include "lib/queue.h"

int currentProcess = 0;
int firstOne = 1;

void initializeTable() {
  int i;
  for (i = 0; i < PROCESSLIMIT; i++) {
    processTable[i].segment = 0x0;
    processTable[i].running = 0;
    processTable[i].name[0] = 0x0;
    processTable[i].stackPointer = 0xFF00;
  }
  procQueueInit();
}

int executeProgram(char *path) {
  char buffer[CSSE_MAX_FSIZE];
  int i, f, bytesRead, segment;
  struct process *currentProcess;

  currentProcess = allocateProcess();
  currentProcess->stackPointer = 0xFF00;

  segment = currentProcess->segment;

  /* Read file and return if it failed. */
  f = fopen(path, 'r');
  bytesRead = fread(f, buffer, CSSE_MAX_FSIZE);
  fclose(f);
  if (!bytesRead)
    return -1;
  println("about to copy program over");
  for (i = 0; i < bytesRead; i++) {
    putInMemory(segment, i, buffer[i]);
  }
  currentProcess->running = 1;
  i = addProc(currentProcess);
  if (i == -1)
    println("Uh oh");
  println("about to init");
  initializeProgram(segment);
  println("inited");
  return 0;
}

/* allocates a process if no segment is given */
struct process *allocateProcess() {
  int i = 0;
  for (i = 0; i < PROCESSLIMIT; i++) {
    if (processTable[i].segment == 0) {
      println("allocating");
      printHex(i);
      processTable[i].segment = TOSEGMENT(i + 2);
      return &processTable[i];
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

/* This function doesn't necessarily make sense. There
   can be multiple processes running at a time, and the
  running flag simply means that it is not to be
  overwritten
*/
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
/* Same problem with this function as described above */
int getRunningSegment() {
	int i;
  for (i = 0; i < PROCESSLIMIT; i++)
    if (processTable[i].running)
      return processTable[i].segment;
}


void handleTimerInterrupt(int segment, int sp) {
  struct process * proc;
  /* Back up the sp in the segment process table entry */
  int temp;
  setKernelDataSegment();
  println("\0");
  println("Before:");
  temp = segment>>12 - 2;
  printhex(temp);
  if (!first)
    processTable[currentProcess].stackPointer = sp;
  first = 0;
  printhex(segment);
  println("\0");

  /* Get the next proc to run from the queue */
  proc = getProc();
  if (proc == 0) {
    println("No procs in queue");
    returnFromTimer(segment, sp);
  }
  /* If it still wants to run, put it back on the queue */
  if (proc->running) {
    addProc(proc);
  }

  /* Copy the new parameters from the new proc */
  segment = proc->segment;
  sp = proc->stackPointer;
  println("After:");
  printHex(segment);
  println("\0");
  restoreDataSegment();
  returnFromTimer(segment, sp);
}
