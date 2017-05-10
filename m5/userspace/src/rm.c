/*
  userspace/src/ls.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
*/

#include <stdio.h>

int main() {
  char args[256], * arg2, * arg1;

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
  puts("Deleting: ");
  puts(arg1);
  puts("\n\r\0");
  fdel(arg1);
  exit();
}