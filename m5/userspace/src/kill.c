/*
  userspace/src/kill.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
*/

#include <stdio.h>
#include <string.h>

int main() {
  char args[256], *arg1;
  int target;

  getArgs(args);
  arg1 = args;
  while(*arg1 != '\0') {
    if (*arg1 == ' ') {
      arg1++;
      break;
    }
    arg1++;
  }
  target = *arg1 - 0x30;

  if (target > 0 && target < 8) {
  	kill(target);
  } else {
  	puts("Invalid process number. Must be between 0 and 7\n\r\0");
  }
  exit();
}
