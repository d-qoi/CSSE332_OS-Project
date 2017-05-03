/*
  Milestone 4 kernel/fs/csse/csse.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */
 
#include "csse.h"
#include "../../vfs.h"
#include "../../io.h"
#include "../../lib/math.h"


void csse_init() {
  int i;
  for (i = 0; i < CSSE_MAX_FOPEN; i++) {
	csse_openFileTable[i].magica = 0x30;
	csse_openFileTable[i].magicb = 0x31;
	csse_openFileTable[i].magicc = 0x32;
	csse_openFileTable[i].magicd = 0x33;
	csse_openFileTable[i].magice = 0x34;
	csse_openFileTable[i].magicf = 0x35;
	csse_openFileTable[i].magicg = 0x36;
	csse_openFileTable[i].magich = 0x37;
    csse_openFileTable[i].open = 0x00;
    csse_openFileTable[i].loadedSectorIndex = -1;
  }
}

/*
 * Opens a file from the file system referenced by openFileIndex
 * 
 * openFileIndex: The index to an openFile.
 * 
 * returns: 0 if successful, -1 if not.
 * */
int csse_fopen(int openFileIndex) {
  int  i, csse_openFileIndex, dnameLen = 0;
  struct csse_openFile * csse_newOpenFile;
  struct openFile * newOpenFile;
  
  newOpenFile = openFileTable + openFileIndex;
  
  /* Find open file slot in table. */
  while (csse_openFileIndex < CSSE_MAX_FOPEN) {
    if (!csse_openFileTable[csse_openFileIndex].open)
      break;
    csse_openFileIndex++;
  }
  if (csse_openFileIndex == CSSE_MAX_FOPEN) {
    return -1; /* Too many files open. */
  }
  
  csse_newOpenFile = csse_openFileTable + csse_openFileIndex;
  
  /* Save values to table */
  csse_newOpenFile->openFileIndex = openFileIndex;
  csse_newOpenFile->loadedSectorIndex = -1;
  
  /* Split the relPath into dname and fname. */
  for (i = 0; i < 256; i++) {
    if (newOpenFile->relPath[i] == '/')
      dnameLen = i + 1;
    if (newOpenFile->relPath[i] == '\0')
      break;
  }

  memset(csse_newOpenFile->dname, 0, 256);
  memcpy(csse_newOpenFile->dname, newOpenFile->relPath, dnameLen);
  memset(csse_newOpenFile->fname, 0, 7);
  memcpy(csse_newOpenFile->fname, newOpenFile->relPath + dnameLen, 6);

  /* Load parent directory into loadedSectorBuffer temporarily. */
  
  println(csse_newOpenFile->dname);
  println("That was supposed to be dname");
  
  csse_newOpenFile->dirSector = csse_readDir(
    mountTable[newOpenFile->mountIndex].drive,
    csse_newOpenFile->dname,
    csse_newOpenFile->loadedSectorBuffer);
    
  while(1);
  if (csse_newOpenFile->dirSector < 0){
    return -1; /* We had a problem reading the directory. */
  }
  
  println(csse_newOpenFile->dname);
  println("That was supposed to be dname 2");
  
  i = csse_findDirEntry(csse_newOpenFile->fname, 
    csse_newOpenFile->loadedSectorBuffer);

  /* Read sector list from directory. */
  memcpy(csse_newOpenFile->sectors, csse_newOpenFile->loadedSectorBuffer + i*32 + 6, 26);

  while(1);
  
  /* We are succesfull! */
  csse_newOpenFile->open = 0xFF;
  return 0;
}



/*
 * Reads data from an open file into the provided buffer.
 * 
 * openFileIndex: The file to read from.
 * buffer: The buffer to read into.
 * count: The number of bytes to read.
 * 
 * returns the number of bytes read, -1 if unsuccesfull.
 * */
int csse_fread(int openFileIndex, char * buffer, int count) {
  int res, bytesRead, csse_openFileIndex;
  struct openFile * currOpenFile;
  struct csse_openFile * csse_currOpenFile;
  
  currOpenFile = openFileTable + openFileIndex;
  
  /* Find csse_openFile */
  for (csse_openFileIndex = 0; csse_openFileIndex < CSSE_MAX_FOPEN; csse_openFileIndex++) {
    if (!csse_openFileTable[csse_openFileIndex].open)
      continue;
    if (csse_openFileTable[csse_openFileIndex].openFileIndex == openFileIndex)
      csse_currOpenFile = csse_openFileTable + csse_openFileIndex;
  }
  if (csse_openFileIndex == CSSE_MAX_FOPEN)
    return -1; /* Did not find csse_openFile! */
  

  /* Iterate over all requested bytes. */
  while (count > 0) {
    /* Load next sector if necessary. */
    if (div(currOpenFile->readWriteIndex, 512) != csse_currOpenFile){
      res = csse_openFileLoadNextSector(csse_openFileIndex);
      if (res < 0)
        break;
    }
    *buffer = csse_currOpenFile->loadedSectorBuffer[mod(currOpenFile->readWriteIndex, 512)];
    buffer++;
    bytesRead++;
    currOpenFile->readWriteIndex++;
    count--;
  }
  return bytesRead;
}


/*
 * Loads the sector at the current rwIndex into loadedSectorBuffer
 * 
 * csse_openFileIndex: An index to a csse_openFile to perform this on.
 * 
 * returns: 0 if succesfull, -1 if not.
 * */
int csse_openFileLoadNextSector(int csse_openFileIndex) {
  int rwIndex, nextSector;
  struct openFile * currOpenFile;
  
  currOpenFile = openFileTable + csse_openFileTable[csse_openFileIndex].openFileIndex;
  
  rwIndex = currOpenFile->readWriteIndex;
  nextSector = csse_openFileTable[csse_openFileIndex].sectors[div(rwIndex, 512)];
  if (nextSector == 0x00)
    return -1; /* No more sectors jumping on the bed! */
  readSectorFrom(
    csse_openFileTable[csse_openFileIndex].loadedSectorBuffer, 
    mountTable[currOpenFile->mountIndex].drive,
    nextSector);
  csse_openFileTable[csse_openFileIndex].loadedSectorIndex = nextSector;
  return 0;
}

/*
 * Reads all filenames in a directory into a provided buffer.
 * 
 * disk: disk number to read from
 * dname: path of directory to read from (must end in /)
 * buffer: A buffer to read the files into.  The buffer should 
 *  be at least 65536 (256*256) bytes long.
 * 
 * returns: number of entries read, -1 if directory not found.
 * 
 * */
int csse_listFilesInDir(int drive, char * dname, char * buffer) {
  char dir[512];
  int res, i, count = 0;
  
  res = csse_readDir(drive, dname, dir);
  if (res < 0)
    return -1;
  
  for (i=0; i < 16; i++) {
    if (dir[i*32] == 0x0)
      continue;
    memcpy(buffer, dir + i*32, 6);
    buffer[6] = '\0';
    buffer += 256;
    count++;
  }
  return count;
}

/*
 * Reads a directory sector from disk into a buffer based on path.
 * 
 * disk: disk number to read from
 * dname: path of directory (must end in /)
 * dirBuffer: 512 byte buffer to read the directory to
 * 
 * returns the sector number of the found directory, -1 if not found
 **/
int csse_readDir(int drive, char * dname, char * dirBuffer) {
  int nextSector;
  
  /* read root directory */
  readSectorFrom(dirBuffer, drive, 2);

  while (*(dname+1) != '\0') {
    int segLen, sector, entryIndex;
    char nameSeg[7];
    char * nextSlash;
    
    nextSlash = strchr(dname+1, '/');
    
    memset(nameSeg, 0, 7);
    segLen = (int)nextSlash - (int)(dname+1);
    if (segLen > 3) {
	}
    memcpy(nameSeg, dname+1, segLen);

    entryIndex = csse_findDirEntry(nameSeg, dirBuffer);
    if (entryIndex < 0) {
      return -1;
    }

    sector = dirBuffer[entryIndex*32 + 6];
    readSectorFrom(&dirBuffer, drive, sector);
    
    dname = nextSlash;
  }
  
  return nextSector;
}

/*
 * Scans a directory buffer for the specified filename. If found, it
 * returns the index in the directory where the filename appears.
 * 
 * fname: A (max 6 byte) filename to look for
 * dir: A dir buffer to look through
 * 
 * returns the directory entry index if found, -1 if not found.
 * */
int csse_findDirEntry(char * fname, char * dir) {
  int equalFname, i, j;
  
  /*println(fname);*/
  
  for (i = 0; i < 16; i++) {
    if (dir[i*32] == 0x0)
      continue;
    equalFname = 1;
    j = 0;
    
    /* Check if the characters in fname match those in the dir entry.*/
    while (fname[j] != '\0') {
      if (j == 6){
        println("Invalid stuff");
        return -1; /* Filename longer than six characters, invalid! */
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
    
    if (equalFname)
      return i;
  }
  println("404, not found.");
  println(fname);
  println(dir);
  return -1; /* Did not find it. */
}


void csse_fclose(int openFileIndex) {
  int csse_openFileIndex;
  struct openFile * currOpenFile;
  struct csse_openFile * csse_currOpenFile;
  
  currOpenFile = openFileTable + openFileIndex;
  
  /* Find csse_openFile */
  for (csse_openFileIndex = 0; csse_openFileIndex < CSSE_MAX_FOPEN; csse_openFileIndex++) {
    if (!csse_openFileTable[csse_openFileIndex].open)
      continue;
    if (csse_openFileTable[csse_openFileIndex].openFileIndex == openFileIndex)
      csse_currOpenFile = csse_openFileTable + csse_openFileIndex;
  }
  if (csse_openFileIndex == CSSE_MAX_FOPEN)
    return; /* Did not find csse_openFile! */
  
  csse_currOpenFile->open = 0x00;
}
