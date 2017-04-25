/*
  Milestone 3 kernel/fs.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */
 
 #include "io.h"
 
/*
 * Reads a file from the filesystem into a buffer.
 * 
 * Returns number of bytes read into buffer.
 **/
int readFile(char * fname, char * buffer) {
  char dir[512];
  int i, j, k, equalFname;
   
  readSector(&dir, 2);
  
  for (i = 0; i < 16; i++) {
    if (dir[i*32] == 0x0)
      continue;
    equalFname = 1;
    j = 0;
    
    /* Check if the characters in fname match those in the dir entry.*/
    while (fname[j] != '\0') {
      if (j == 6){
        return 0; /* Filename longer than six characters, invalid! */
      }
      if (dir[i*32+j] != fname[j]) {
        equalFname = 0;
        break;
      }
      j++;
    }

    /* Check that the remaining characters in the dir entry are zero. */
    while (j < 6) {
      if (j == 6){
        println("j is 6");
      }
      if (dir[i*32+j] != '\0'){
        equalFname = 0;
        break;
      }
      j++;
    }
    
    /* Read the file sector by sector. */
    if (equalFname){
      for (k = 0; k < 26; k++) {
        if (dir[i*32 + 6 + k] == 0x0)
          break;
        readSector(buffer + 512*k, dir[i*32 + 6 + k]);
      }
      return k*512;
    }
  }
  return 0;
}
