/*
  kernel/processing.h
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */


#define PROCESSNAMELIMIT 15
#define PROCESSLIMIT 32

#define TOSEGMENT(I) I<<12;	/* segment is 0xN000 */
#define TOINT(S) S>>12;		

int executeProgram(char * path, int segment);

struct process* allocateProcess();
struct process* reallocateProcess(int segment);
struct process* freeProcess(int segment);
struct process* getProcessByName(char * name);
struct process* getCurrentProcess();

struct process {
	int segment;
	char running;
	char name[PROCESSNAMELIMIT];
};

struct process processTable[PROCESSLIMIT];


