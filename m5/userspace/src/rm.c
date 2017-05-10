/*
  userspace/src/ls.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
*/

#include <stdio.h>

int main() {
  char args[256], * arg2;

  getArgs(args);
  while(*args != '\0') {
    if (*args == ' ') {
      args++;
      break;
    }
    args++;
  }
  arg2 = args;
  while(*arg2 != '\0') {
    if (*arg2 == ' ') {
      arg2 = '\0';
      break;
    }
    arg2++;
  }
  puts("Deleting: ");
  puts(args);
  puts("\n\r\0");
  fdel(args);
}