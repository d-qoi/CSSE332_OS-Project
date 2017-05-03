/*
  Milestone 4 kernel/vfs.h
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */
 

#ifndef _VFS_H_
#define _VFS_H_

#define MAX_MOUNTS 4
#define MAX_FOPEN 16

#define FS_CSSE 0

struct mount {
  char drive;
  char path[255];
  int fsType;
};

struct openFile {
  char open;
  char mountIndex;
  char relPath[256];
  int readWriteIndex;
  char mode;
};

int mountCount = 0;
struct mount mountTable[MAX_MOUNTS];
struct openFile openFileTable[MAX_FOPEN];

int mountfs(char drive, char * path, int fsType);
int fopen(char * path, char mode);
int fread(int openFileIndex, char * buffer, int count);
int freaddir(char * path, char * buffer);
int fwrite(int openFileIndex, char * buffer, int count);
int fclose(int openFileIndex);


#endif
