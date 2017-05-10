/*
  userspace/src/cat.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
*/

#include <stdio.h>
#include <string.h>

int main() {
  char args[256], file[13056];
  char * arg2;
  int fp;

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

  fp = fopen(args,'r');
  if (fp < 0) {
    puts("Unable to open file: ");
    puts(args);
    puts("\n\r\0");
  } else {
    fread(fp, file, sizeof(file));
    puts(file);
    puts("\n\r\0");
  }
}