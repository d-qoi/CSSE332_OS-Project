/*
  userspace/src/cat.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
*/

#include <stdio.h>
#include <string.h>

int main() {
  char args[256], file[13056];
  char * arg1, * arg2;
  int fp;

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

  fp = fopen(arg1,'r');
  if (fp < 0) {
    puts("Unable to open file: ");
    puts(arg1);
    puts("\n\r\0");
  } else {
    fread(fp, file, sizeof(file));
    puts(file);
    puts("\n\r\0");
  }
  fclose(fp);
  exit();
}