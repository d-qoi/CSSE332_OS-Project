/*
  userspace/src/ls.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
*/

#include <stdio.h>
#include <string.h>

int main() {
  char args[256], * arg2;
  char files[13056];
  int i, len;

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
  puts("Directory List for: ")
  puts(args);
  puts("\n\r\0");
  getDirList(args, files);
  for(i = 0; i < 50; i++) {
    len = strlen(&files[i * 256]);
    if (len == 0) {
      break;
    } else {
      puts(&files[i * 256]);
      puts("\n\r\0");
    }
  }
}