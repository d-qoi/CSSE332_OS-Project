/*
  Milestone 4 kernel/fs/csse/csse.h
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */
 

#ifndef _CSSE_H_
#define _CSSE_H_

#define CSSE_MAX_FSIZE 13312
#define CSSE_MAX_FOPEN 8

struct csse_openFile {
  char open; /* 0x00 for not open/memory available, 0xFF for open/memory used. */
  int dirSector; /* Sector of owning directory. */
  char dname[256]; /* Name (full path with leading and trailing '/') of parent directory. */ 
  char fname[7]; /* Name of file (6 bytes no '/') within parent directory. */
  char sectors[26]; /* Sectors containing the file info. */
  int loadedSectorIndex; /* Index of the loaded sector in the above list. */
  char loadedSectorBuffer[512]; /* Buffer of the loaded sector referenced by loadedSectorIndex. */
  int openFileIndex; /* Maps to an instance of openFile */
  int sectorWrites; /* Have any writes occured to the loaded sector? */
};

struct csse_openFile csse_openFileTable[CSSE_MAX_FOPEN];

void csse_init();
int csse_fopen(int openFileIndex);
int csse_fread(int openFileIndex, char * buffer, int count);
int csse_fwrite(int openFileIndex, char * buffer, int count);
int csse_openFileLoadNextSector(int csse_openFileIndex);
int csse_listFilesInDir(int disk, char * dname, char * buffer);
int csse_readDir(int disk, char * dname, char * dirBuffer);
int csse_findDirEntry(char * fname, char * dir);
int csse_freaddir(char * path, char * buffer);
void csse_fclose(int openFileIndex);

#endif
