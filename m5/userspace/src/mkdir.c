/*
  userspace/src/mkdir.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
*/

#include <stdio.h>
#include <string.h>

int main() {
  char args[256];
  char * arg2;
  char * arg1;
  int len;

  getArgs(args);
  arg1 = args;
  while(*arg1 != '\0') {
    if (*arg1 == ' ') {
      arg1++;
      break;
    }
    arg1++;
  }
  arg2 = arg1;
  while(*arg2 != '\0') {
    if (*arg2 == ' ') {
      arg2 = '\0';
      break;
    }
    arg2++;
  }

  len = strlen(arg1);
  if (len != 0 && *(arg2-1) == '/')
    fmkdir(arg1);
  else {
    puts("Unable to make file: ");
    puts(arg1);
    puts("\n\r\0");
  }
  exit();
}
