/*
  Milestone 3 fs.c file
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */
 
 #include "interrupt.h"
 
 void readFile(char * fname, char * buffer) {
	char dir[512];
	int i, j, k, equal;
	
	readSector(&dir, 2);
	
	for (i = 0; i < 16; i++) {
		if (dir[i*32] == 0x0)
			continue;
		equal = 1;
		for (j = 0; j < 6; j++) {
			if (dir[i*32+j] != fname[j]) {
				equal = 0;
				break;
			}
		}
		if (equal){
			for (k = 0; k < 26; k++) {
				if (dir[i*32 + 6 + k] == 0x0)
					break;
				readSector(&buffer + 512*k, dir[i*32 + 6 + k]);
			}
			return;
		}
	}
 }
