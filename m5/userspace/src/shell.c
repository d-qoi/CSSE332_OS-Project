/*
  userspace/src/shell.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */

#include <stdio.h>
#include <string.h>

#define HARDCODE 1

int main() {
  char cmdBuff[256], pathBuff[512], fileBuff[13000], temp[256];
  int i, j, k, equals, ind = 0;
  int  c, len, place = 0;
  enableInterrupts();
  while(1) {
    for(i = 0; i < 256; i++) {
      cmdBuff[i] = 0;
    }
    cmdBuff[0] = 0;
    pathBuff[0] = 0;
    fileBuff[0] = 0;
    puts("0xG00: ");
    gets(cmdBuff);
    removeLinefeed(cmdBuff);

    if (HARDCODE) {
      if (!strncmp("type", cmdBuff, 4)) {
        if (cmdBuff[5] != '/' || cmdBuff[6] < 'A') {
          puts("Invalid arguments to type command\n\r");
          /* exit(); */
        }
        len = strlen(&cmdBuff[5]);
        cmdBuff[5 + len] = '\0';
        ffread(&cmdBuff[5], fileBuff);
        puts(fileBuff);
        /* exit(); */
      } else if(!strncmp("execute", cmdBuff, 7)) {
        if (cmdBuff[8] != '/' || cmdBuff[9] < 'A') {
          puts("Invalid arguments to execute command\n\r");
        } else {
          exec(&cmdBuff[8], cmdBuff);
        }
        /* exit(); */
        /*interrupt(0x21, 4, cmdBuff, 0x2000, 0);*/

      } else if (!strncmp("delete", cmdBuff, 6)) {
        if (cmdBuff[7] != '/' || cmdBuff[8] < 'A') {
          puts("Invalid arguments to delete command\n\r");
          /* exit(); */
        }
        len = strlen(&cmdBuff[7]);
        cmdBuff[7 + len] = '\0';
        fdel(&cmdBuff[7]);
      } else if (!strncmp("copy", cmdBuff, 4)) {
        if(cmdBuff[5] != '/' || cmdBuff[6] < 'A') {
          puts("Invalid inputs to copy command\n\r");
          /* exit(); */
        }

        c = 5;
        while(1) {
          if(cmdBuff[c] == ' ') {
            cmdBuff[c] = '\0';
            break;
          }
          if(cmdBuff[c] == '\0') {
            puts("Invalid output file\n\r");
            /* exit(); */
          }
          c++;
        }
        c++;
        ffread(&cmdBuff[5], fileBuff);

        if(cmdBuff[c] != '/' || cmdBuff[c + 1] < 'A') {
          puts("Invalid output file\n\r");
          /* exit(); */
        }
        len = strlen(&cmdBuff[c]);
        cmdBuff[c + len] = '\0';
        ffwrite(&cmdBuff[c], fileBuff);

      } else if (!strncmp("dir", cmdBuff, 3)) {
        if (cmdBuff[4] != '/') {
          puts("Invalid arguments to dir command\n\r");
          /* exit(); */
        }
        len = strlen(&cmdBuff[4]);
        if (cmdBuff[3+len] != '/'){
          len++;
          cmdBuff[3+len] = '/';
        } 
        cmdBuff[4 + len] = '\0';
        getDirList(&cmdBuff[4], fileBuff);
        for (i = 0; i < 13000; i += 256) {
          len = strlen(&fileBuff[i]);
          if (len == 0) {
            break;
          } else {
            puts(&fileBuff[i]);
            puts("\r\n\0");
          }
        }
      } else if (!strncmp("create", cmdBuff, 6)) {
        if (cmdBuff[7] != '/' || cmdBuff[8] < 'A') {
          puts("Invalid arguments to create command\n\r");
          /* exit(); */
        }
        len = strlen(&cmdBuff[7]);
        cmdBuff[7 + len] = '\0';
        while(1) {
          gets(temp);
          len = strlen(temp);
          if (len == 2) {
            ffwrite(&cmdBuff[7], fileBuff);
            break;
          } else {
            for(i = 0; i < len; i++) {
              fileBuff[place++] = temp[i];
            }
          }
        }
      } else if (!strncmp("kill", cmdBuff, 4)) {
        if (cmdBuff[5] > 55 || cmdBuff[5] < 48) {
          puts("Invalid process number. Must be between 0 and 7\n\r");
        } else {
          kill(cmdBuff[5] - 48);
        }
      } else if (!strncmp("execforeground", cmdBuff, 14)) {
        if (cmdBuff[15] != '/' || cmdBuff[16] < 'A') {
          puts("Invalid arguments to execute command\n\r");
        } else {
          exec(&cmdBuff[15], 1);
        }
      } else if (!strncmp("mkdir", cmdBuff, 3)) {
        if (cmdBuff[6] != '/') {
          puts("Invalid arguments to mkdir command\n\r");
          /* exit(); */
        }
        len = strlen(&cmdBuff[6]);
        if (cmdBuff[5+len] != '/'){
          len++;
          cmdBuff[5+len] = '/';
        } 
        cmdBuff[6 + len] = '\0';
        fmkdir(&cmdBuff[6]);
      } else {
        puts("Unknown command: ");
        puts(cmdBuff);
        puts("\n\r");
        /* exit(); */
      }
    } else {
      getDirList("/bin", pathBuff);
      for (i = 0; i < 16; i++) {
        equals = 1;
        for (j = 0; j < 6; j++) {
          if (cmdBuff[j] != pathBuff[i * 6 + j]) {
            equals = 0;
            break;
          }
        }
        if (equals) {
          char cmd[16], args[16];
          memset(cmd, 0, 16);
          memset(args, 0, 16);
          strcat(cmd, "/bin/");
          memcpy(cmd + 5, cmdBuff, 6);
          memcpy(args, cmdBuff + 6);
          puts(args);
          exec(cmd);
        }
      }
      puts("Unknown command: ");
      puts(cmdBuff);
      puts("\n\r");

      /* exit(); */
    }
  }
}
