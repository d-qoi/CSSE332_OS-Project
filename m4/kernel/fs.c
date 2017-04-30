/*
  Milestone 3 kernel/fs.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */
 
 #include "io.h"
 #include "lib/math.h"
 
 
int listFilesInDir(char * path, char * buffer) {
  char dir[512], nameSeg[7];
  int res, segLen, i, found;
  
  /* read root directory */
  readSector(&dir, 2);

  while (*path != '\0') {
    memset(nameSeg, 0, 7);
    path = strchr(path, '/') + 1;
    segLen = (int)strchr(path, '/') - (int)path;
    memcpy(nameSeg, path, segLen);
    res = readFile(nameSeg, buffer, dir);
    if (!res)
      return 0;
    path += segLen;
    memcpy(dir, buffer, 512);
  }
  
  found = 0;
  for (i=0; i < 16; i++) {
    if (dir[i*32] == 0x0)
      continue;
    memcpy(buffer, dir + i*32, 6);
    buffer += 6;
    found++;
  }
  return found;
}
 
/*
 * Reads a file from a path.
 * 
 * Returns the number of bytes read into buffer.
 * */
int readFileFromPath(char * path, char * buffer) {
  char dir[512], nameSeg[7];
  int res, segLen, i;
  
  /* read root directory */
  readSector(&dir, 2);

  while (*path != '\0') {
    memset(nameSeg, 0, 7);
    path = strchr(path, '/') + 1;
    segLen = (int)strchr(path, '/') - (int)path;
    memcpy(nameSeg, path, segLen);
    res = readFile(nameSeg, buffer, dir);
    if (!res)
      return 0;
    path += segLen;
    if (*path != '\0') {
      memcpy(dir, buffer, 512);
    }
  }
  return res;
}

/*
 * Reads a file from the filesystem into a buffer.
 * 
 * Returns the number of bytes read into buffer.
 **/
int readFile(char * fname, char * buffer, char * dir) {
  int i, j, k, equalFname, sector;
  
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
