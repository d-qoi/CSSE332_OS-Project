/*
  userspace/src/create.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
*/
#include <stdio.h>
#include <string.h>

int main() {
  char args[256], buffer[13312], temp[255];
  char *arg2, *arg1;
  int len, i, place = 0;;

  getArgs(args);
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
  if (arg1[0] == "/" && arg1[1] >= 'A') {
    while(1) {
      gets(temp);
      len = strlen(temp);
      if (len == 2) {
        i = fopen(arg1, 'w');
        fwrite(i, buffer, sizeof(buffer) - 1);
        fclose(i);
        return;
      } else {
        for (i = 0; i < len; i++) {
          buffer[place++] = temp[i];
        }
      }
    }
  } else {
    puts("Please specify a valid file neame. \n\r\0");
  }
  exit();
}