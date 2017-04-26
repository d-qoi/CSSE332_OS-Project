/*
  Milestone 3 userspace/src/shell.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */

#include <stdio.h>
#include <string.h>

int main() {
  char cmdBuff[256], pathBuff[512];
  int i, j, equals;
  
  puts("# ");
  gets(cmdBuff);
  
  getDirList("/bin", pathBuff);
  
  for (i = 0; i < 16; i++) {
    equals = 1;
    for (j = 0; j < 6; j++) {
      if (cmdBuff[j] != pathBuff[i*6 + j]){
        equals = 0;
        break;
      }
    }
    if (equals) {
      char cmd[16], args[16];
      memset(cmd, 0, 16);
      memset(args, 0, 16);
      strcat(cmd, "/bin/");
      memcpy(cmd+5, cmdBuff, 6);
      memcpy(args, cmdBuff+6);
      puts(args);
      exec(cmd, args);
    }
  }
  
  puts("Unknown command: ");
  puts(cmdBuff);
  puts("\n\r");
  exit();
}
