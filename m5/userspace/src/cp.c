/*
  userspace/src/cp.c
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
  int fp1, fp2, bytes;
  getArgs(args);

  for(i = 0; i < 256; i++) {
    switch (j) {
    case 0:
      if (args[i] == ' ' && args[i + 1] == '/') {
        j = 1;
        arg1 = &args[i + 1];
        args[i] = '\0'
      }
      break;
    case 1:
      if (args[i] == ' ' && args[i + 1] == '/') {
        j = 2;
        arg2 = &args[i + 1];
        args[i] = '\0'
      }
      break;
    case 2:
      if (args[i] == ' ') {
        args[i] = '\0';
      }
    }
  }
  if(j != 2) {
    puts("Please specify the source file and the dest file.\n\r\0");
    return;
  }
  fp1 = fopen(arg1, 'r');
  fp2 = fopen(arg2, 'w');

  if(fp1 < 0) {
    puts("Unable to open: ");
    puts(arg1);
    puts("\n\r\0");
    return;
  }
  if (fp2 < 0) {
    puts("Unable to open: ");
    puts(arg2);
    puts("\n\r\0");
    return;
  }

  bytes = 1024;
  while(bytes == 1024) {
    bytes = fread(fp1, buffer, bytes);
    fwrite(fp2, buffer, bytes);
  }

}

