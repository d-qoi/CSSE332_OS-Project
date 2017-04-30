/*
  Milestone 3 userspace/src/shell.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */

#include <stdio.h>
#include <string.h>

#define HARDCODE 1

int main() {
  char cmdBuff[256], pathBuff[512], fileBuff[13000], type[5], execute[8], temp;
  int i, j, k, equals, ind = 0;
  for(k = 0; k < 256; k++) {
    cmdBuff[k] = '\0';
  }
  fileBuff[0] = 0;
  puts("# ");
  gets(cmdBuff);
  removeLinefeed(cmdBuff);

  type[0] = 't';
  type[1] = 'y';
  type[2] = 'p';
  type[3] = 'e';
  type[4] = '\0';
  
  execute[0] = 'e';
  execute[1] = 'x';
  execute[2] = 'e';
  execute[3] = 'c';
  execute[4] = 'u';
  execute[5] = 't';
  execute[6] = 'e';
  execute[7] = '\0';
      
  if (HARDCODE) {
    if (!strncmp(type, cmdBuff, 4)) {
      interrupt(0x21, 3, &cmdBuff[5], fileBuff, 0);
      interrupt(0x21, 0, fileBuff, 0, 0);
      exit();
    } else if(!strncmp(execute, cmdBuff, 7)) {
      interrupt(0x21, 4, cmdBuff, 0x2000, 0);
      
    }else {
      puts("Unknown command: ");
      puts(cmdBuff);
      puts("\n\r");
      exit();
    }
  } else {
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
	    exec(cmd);
      }
    }
    puts("Unknown command: ");
    puts(cmdBuff);
	puts("\n\r");
	exit();
  }
}
