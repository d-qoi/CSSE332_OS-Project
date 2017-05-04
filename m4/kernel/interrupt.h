/*
  Milestone 3 kernel/interrupt.h
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */

#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#define PROCESSNAMELIMIT 15
#define PROCESSLIMIT 32

/*
Definitions of interrupt commands
*/
int executeProgram(char *, int);
void handleInterrupt21(int, int, int, int);

struct process* allocateProcess();
struct process* freeProcess(int segment);
struct process* getCurrentProcess();

struct process {
	int segment;
	char running;
	char name[PROCESSNAMELIMIT];
};

struct processTable process[PROCESSLIMIT];

#endif

