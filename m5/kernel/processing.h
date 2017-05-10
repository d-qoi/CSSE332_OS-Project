/*
  kernel/processing.h
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */

#ifndef _PROCESSING_H_
#define _PROCESSING_H_

#define PROCESSNAMELIMIT 15
#define PROCESSLIMIT 32

#define TOSEGMENT(I) I<<12	/* segment is 0xN000 */
#define TOINT(S) S>>12

#define GRS getRunningSegment()

int executeProgram(char *path, char *args, int shouldWait);

int allocateProcess();
int getRunningSegment();
struct process getRunningProcess();
void passArgsOfCurrent(char * buffer);
void terminate(int proc);
void clearWait(int proc);
void handleTimerInterrupt(int segment, int sp);

struct process {
  int sp;
  char running;
  int waiting;
  char name[PROCESSNAMELIMIT];
  char args[256];
};

struct process processTable[PROCESSLIMIT];

int currentProcess = 0;

#endif
