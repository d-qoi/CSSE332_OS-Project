
#include "../interrupt.h"
#include "../processing.h"
#include "./queue.h"

Queue q;
struct process * procArray[PROCESSLIMIT];

void procQueueInit() {
  q.head = 0;
  q.tail = 0;
}

int addProc(struct process * proc) {
  if ((q.tail + 1) == PROCESSLIMIT) {
    /* Tail needs to wrap, so check if head is zero */
    if (q.head == 0) {
      /* If so, it's full */
      return -1;
    } else {
      /* If not, then add the proc ptr */
      procArray[q.tail] = proc;
      q.tail = 0;
      return 0;
    }
  } else {
    /* Normal tail increment */
    if ((q.tail + 1) == q.head) {
      /* If equal after incrementing tail, then full */
      return -1;
    } else {
      /* Otherwise, increment and store new proc ptr */
      procArray[q.tail++] = proc;
      return 0;
    }
  }
}

struct process * getProc(){
  struct process * temp;
  temp = NULL;
  if (q.head == q.tail) {
    /* If equal, then queue is empty */
    return temp;
  } else {
    /* Get the next ptr, and wrap head if necessary */
    temp = procArray[q.head++];
    if (q.head == PROCESSLIMIT)
      q.head = 0;
    return temp;
  }
}
