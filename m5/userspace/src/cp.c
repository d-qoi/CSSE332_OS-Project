/*
  userspace/src/cat.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
*/

#include <stdio.h>
#include <string.h>

int main() {
  char args[256];
  char buffer[1024];
  char *arg1;
  char *arg2;
  int i, j = 0;

  getArgs(args);

  for(i = 0; i < 256; i++) {
    switch (j) {
    case 0:
      if (args[i] == ' ' && args[i + 1] == '/')
        j = 1;
      break;
    case 1:
      if (args[i] == ' ' && args[i + 1] == '/')
        j = 2;
      break;
    case 2:
      break;
    }
  }

  if(j != 2) {
    puts("Please specify the source file and the dest file.\n\r\0");
  }