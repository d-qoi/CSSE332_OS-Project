#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "../interrupt.h"

#ifndef NULL
#define NULL 0
#endif

typedef struct {
  int head;
  int tail;
} Queue;

/* Initializes the process queue */
void procQueueInit();

/* Adds a process to the process queue.
   If no space available, returns -1,
   otherwise returns 0
*/
int addProc(struct process * proc);

/* Returns a process pointer from the head
   of the process queue.
   Returns NULL if the queue is empty
*/
struct process *  getProc();
	    
#endif
