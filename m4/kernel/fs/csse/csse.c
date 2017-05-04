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
  csse_newOpenFile->sectorWrites = 0x00;
  
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
  
  csse_newOpenFile->dirSector = csse_readDir(
    mountTable[newOpenFile->mountIndex].drive,
    csse_newOpenFile->dname,
    csse_newOpenFile->loadedSectorBuffer,
    newOpenFile->mode == 'w');
  
  if (csse_newOpenFile->dirSector < 0){
    return -1; /* We had a problem reading the directory. */
  }

  
  i = csse_findDirEntry(csse_newOpenFile->fname, 
    csse_newOpenFile->loadedSectorBuffer);

  /* Read sector list from directory. */
  memcpy(csse_newOpenFile->sectors, csse_newOpenFile->loadedSectorBuffer + i*32 + 6, 26);
  
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
    if (csse_openFileTable[csse_openFileIndex].openFileIndex == openFileIndex){
      csse_currOpenFile = csse_openFileTable + csse_openFileIndex;
      break;
    }
  }
  if (csse_openFileIndex == CSSE_MAX_FOPEN) {
    println("Did not find stuff");
    return -1; /* Did not find csse_openFile! */
  }

  /* Iterate over all requested bytes. */
  while (count > 0) {
    /* Load next sector if necessary. */
    if (div(currOpenFile->readWriteIndex, 512) != csse_currOpenFile->loadedSectorIndex){
      /* Write current sector if modified. */
      if (csse_currOpenFile->sectorWrites && currOpenFile->mode == 'w')
        csse_openFileWriteCurrSector(csse_openFileIndex);
      /* Load next sector. */
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
 * Writes data from a provide buffer into an open file.
 * 
 * openFileIndex: The file to write from.
 * buffer: The buffer to write into.
 * count: The number of bytes to write.
 * 
 * returns the number of bytes read, -1 if unsuccesfull.
 * */
int csse_fwrite(int openFileIndex, char * buffer, int count) {
  int res, bytesWritten, csse_openFileIndex;
  struct openFile * currOpenFile;
  struct csse_openFile * csse_currOpenFile;
  
  currOpenFile = openFileTable + openFileIndex;
  
  /* Find csse_openFile */
  for (csse_openFileIndex = 0; csse_openFileIndex < CSSE_MAX_FOPEN; csse_openFileIndex++) {
    if (!csse_openFileTable[csse_openFileIndex].open)
      continue;
    if (csse_openFileTable[csse_openFileIndex].openFileIndex == openFileIndex){
      csse_currOpenFile = csse_openFileTable + csse_openFileIndex;
      break;
    }
  }
  if (csse_openFileIndex == CSSE_MAX_FOPEN) {
    println("Did not find stuff");
    return -1; /* Did not find csse_openFile! */
  }

  /* Iterate over all requested bytes. */
  while (count > 0) {
    
    /* Load next sector if necessary. */
    if (div(currOpenFile->readWriteIndex, 512) != csse_currOpenFile->loadedSectorIndex){
      
      /* Write current sector. */
      if (csse_currOpenFile->sectorWrites && currOpenFile->mode == 'w')
        csse_openFileWriteCurrSector(csse_openFileIndex);
      /* Read next sector. */
      res = csse_openFileLoadNextSector(csse_openFileIndex);
      if (res < 0){
        println("Gen new sector!");
        /* Generate new sector. */
        res = csse_openFileAssignNewSector(
          mountTable[currOpenFile->mountIndex].drive, 
          csse_currOpenFile->dirSector, 
          csse_currOpenFile->fname);
        if (res < 0) {
          println("Cannot gen new sector!");
          break; /*Cannot assign new sector.*/
        }
        csse_currOpenFile->sectors[csse_currOpenFile->loadedSectorIndex+1] = res;
        res = csse_openFileLoadNextSector(csse_openFileIndex);
        if (res < 0)
          println("Wat, I thought I fixed that..");
      }
    }
    csse_currOpenFile->loadedSectorBuffer[mod(currOpenFile->readWriteIndex, 512)] = *buffer;
    csse_currOpenFile->sectorWrites = 0xFF;
    buffer++;
    bytesWritten++;
    currOpenFile->readWriteIndex++;
    count--;
  }
  return bytesWritten;
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
  csse_openFileTable[csse_openFileIndex].loadedSectorIndex = div(rwIndex, 512);
  csse_openFileTable[csse_openFileIndex].sectorWrites = 0;
  return 0;
}

/*
 * Find and assign a new sector to an open file.
 * 
 * drive: A drive ID.
 * dirSector: The sector of the directory block
 * fname: A filename
 * 
 * */
int csse_openFileAssignNewSector(int drive, int dirSector, char * fname) {
  char mapBuffer[512], dirBuffer[512];
  struct openFile * currOpenFile;
  int dirEntry, sectorSlot, newSector;
    
  /* Read map. */
  readSectorFrom(mapBuffer, drive, 1);

  /* Read dir */
  readSectorFrom(dirBuffer, drive, dirSector);
  
  /* Check if there is a slot for a new sector in the directory. */
  for (sectorSlot = 0; sectorSlot < 26; sectorSlot++) {
    if (dirBuffer[dirEntry*32 + 6 + sectorSlot] == 0x00)
      break;
  }
  if (sectorSlot == 26)
    return -1;
  
  /* Find an available sector. */
  for (newSector = 0; newSector < 512; newSector++) {
    if (mapBuffer[newSector] == 0x00)
      break;
  }
  if (newSector == 512)
    return -1;

  dirEntry = csse_findDirEntry(fname, dirBuffer);
  
  /* Reserve sector. */
  mapBuffer[newSector] = 0xFF;
  dirBuffer[dirEntry*32 + 6 + sectorSlot] = newSector;
  
  /* Save map. */
  writeSectorTo(mapBuffer, drive, 1);
  
  /* Save dir */
  writeSectorTo(dirBuffer, drive, dirSector);
  
  return newSector;
}

/*
 * Loads the sector at the current rwIndex into loadedSectorBuffer
 * 
 * csse_openFileIndex: An index to a csse_openFile to perform this on.
 * 
 * returns: 0 if succesfull, -1 if not.
 * */
int csse_openFileWriteCurrSector(int csse_openFileIndex) {
  int rwIndex, currSector;
  struct openFile * currOpenFile;
  
  println("Writing a sector!");
  
  currOpenFile = openFileTable + csse_openFileTable[csse_openFileIndex].openFileIndex;
  currSector = csse_openFileTable[csse_openFileIndex].sectors[csse_openFileTable[csse_openFileIndex].loadedSectorIndex];
  
  rwIndex = currOpenFile->readWriteIndex;

  writeSectorTo(csse_openFileTable[csse_openFileIndex].loadedSectorBuffer,
    mountTable[currOpenFile->mountIndex].drive, currSector);

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
  
  res = csse_readDir(drive, dname, dir, 0);
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
 * create: Create the directory if it is not found.
 * 
 * returns the sector number of the found directory, -1 if not found
 **/
int csse_readDir(int drive, char * dname, char * dirBuffer, int create) {
  int nextSector = 2;
  
  /* read root directory */
  readSectorFrom(dirBuffer, drive, 2);

  while (*(dname+1) != '\0') {
    int segLen, entryIndex;
    char nameSeg[7];
    char * nextSlash;
    
    nextSlash = strchr(dname+1, '/');
    
    memset(nameSeg, 0, 7);
    segLen = (int)nextSlash - (int)(dname+1);

    memcpy(nameSeg, dname+1, segLen);

    entryIndex = csse_findDirEntry(nameSeg, dirBuffer);
    if (entryIndex < 0) {
      if (!create)
        return -1;
      else {
        int i;
        for (i = 0; i < 32; i++){
          if (dirBuffer[i*32] == 0)
            break;
        }
        if (i == 32)
          return -1;
      }
    }

    nextSector = dirBuffer[entryIndex*32 + 6];
    readSectorFrom(dirBuffer, drive, nextSector);
    
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
    if (csse_openFileTable[csse_openFileIndex].openFileIndex == openFileIndex){
      csse_currOpenFile = csse_openFileTable + csse_openFileIndex;
      break;
    }
  }
  if (csse_openFileIndex == CSSE_MAX_FOPEN) {
    println("Did not find stuff");
    return; /* Did not find csse_openFile! */
  }
  
  /* Write to sector if the buffer has been modified. */
  if (csse_currOpenFile->sectorWrites && currOpenFile->mode == 'w')
    csse_openFileWriteCurrSector(csse_openFileIndex);
  
  csse_currOpenFile->open = 0x00;
}
