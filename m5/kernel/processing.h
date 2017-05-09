/*
  kernel/processing.h
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */


#define PROCESSNAMELIMIT 15
#define PROCESSLIMIT 32

#define TOSEGMENT(I) I<<12	/* segment is 0xN000 */
#define TOINT(S) S>>12

#define GRS getRunningSegment()

int executeProgram(char *path, int segment);

int allocateProcess();
int getRunningSegment();
void terminate(int proc);
void clearWait(int proc);
void handleTimerInterrupt(int segment, int sp);

struct process {
  int sp;
  char running;
  int waiting;
  char name[PROCESSNAMELIMIT];
};

struct process processTable[PROCESSLIMIT];

int currentProcess = 0;

