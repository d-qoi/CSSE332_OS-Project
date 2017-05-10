/*
  userspace/src/mrepl.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */

#include <stdio.h>
#include <string.h>

int main() {
  while(1) {
    char cmd[256], cmdBuff[256];
    int i;
    int k;
    memset(cmd, '\0', 256);
    memcpy(cmd, "/bin/", 5);
    i = 0;
    while(cmdBuff[i] != '\0' && i < 256) { /* if the command has args, or spaces at the end */
      if (cmdBuff[i] == ' ') {
        cmdBuff[i] = '\0';
        strcat(cmd, cmdBuff);
        cmdBuff[i] = ' ';
        break;
      }
      i++;
    }
    if (cmdBuff[i] == '\0') { /* if the command has no args and no spaces */
      strcat(cmd, cmdBuff);
    }
    puts(cmdBuff);
    puts("\n\r");
    puts(cmd);
    puts("\n\r");
    k = 1;
    for(i = 0; i < 256; i++) {
      if (cmdBuff[i] == '&') {
        k = 0;
        break;
      }
    }
    if (k) {
      puts("running in forground\n\r\0");
    }
    if (strncmp(cmd, "/bin/exit", 9)) {
      exit();
    }
    exec(cmd, cmdBuff, k);
  }
}
