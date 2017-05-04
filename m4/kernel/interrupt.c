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

/*TODO: Move this elsewhere when we have a good spot for it.*/
int executeProgram(char * path, int segment) {
  char buffer[CSSE_MAX_FSIZE];
  int i, bytesRead;

  struct process* currentProcess;

  if (segment == 0) {
    currentProcess = allocateProcess();
    if (currentProcess == 0) { /* in the case that we are not able to allcate another process */
      return -1;
    }
  } else {
    currentProcess = reallocateProcess(segment);
  }

  segment = currentProcess->segment;

  /* this may need to be changed later to handle multiple processes */
  currentProcess->running = 1;
  currentProcess->name = "Hello World";

  /* Read file and return if it failed. */
  bytesRead = fread(path, buffer, CSSE_MAX_FSIZE);
  if (!bytesRead) 
    return -1;
  
  for (i = 0; i < bytesRead; i++) {
    putInMemory(segment, i, buffer[i]);
  }
  
  launchProgram(segment);
  return 0;
}

struct process* allocateProcess() {
  int i = 0;
  for (i = 0; i<PROCESSLIMIT; i++) {
    if (processTable[i].segment == 0) {
      processTable[i].segment = i+2;
      return processTable[i];
    }
  }
  return 0;
}

struct process* reallocateProcess(int segment) {
  if ((segment - 2 < 0) || (segment - 2 > PROCESSLIMIT - 1))
    return processTable[segment - 2];
  return allocateProcess();
}

struct process* freeProcess(int segment) {
  if ((segment - 2 < 0) || (segment - 2 > PROCESSLIMIT - 1)) {
    processTable[segment -2].segment = 0;
    return processTable[segment-2];
  }
  return 0;
}

struct process* getProcessByName(char * name) {
  int i; 
  for (i = 0; i<PROCESSLIMIT; i++) 
    if (strcmp(name, processTable[i].name))
      return processTable[i];
  return 0;
}

struct process* getCurrentProcess() {
  int i;
  for (i = 0; i<PROCESSLIMIT; i++)
    if (processTable[i].running)
      return processTable[i];

  /* if it is unable to find a running process */
  processTable[0].running = 1;
  return processTable[0];

}

void terminate() {
  interrupt(0x21, 4, "/bin/shell", 0x2000, 0);
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
  int f, bytesRead;
      
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
      f = fopen((char *) bx, 'r');
      bytesRead = fread(f, (char *) cx, CSSE_MAX_FSIZE);
      fclose(f);

      if (bytesRead == 0) {
        char * msg = "ERROR: readFile read 0 bytes from ";
        strcat(msg, bx);
        println(msg);
      }
      break;
    case 4: 
      /* Execute program at filename *bx in segment cx*/
      executeProgram((char *) bx, cx);
      break;
    case 5: /* Terminate current program. */
      terminate();
      break;
    case 6: /* Write a sector */
      writeSector((char *)bx, cx);
      break;
    case 7: /* Delete a file */
      break;
    case 8: /* Write a file */
      break;
    case 9:
      freaddir((char *) bx, (char *) cx);
      break;
  }
}

void handleTimerInterrupt(int segment, int sp){
  returnFromTimer(segment, sp);
}
