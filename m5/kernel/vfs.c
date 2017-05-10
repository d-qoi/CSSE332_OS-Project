/*
  Milestone 4 kernel/vfs.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */
 
#include "io.h"
#include "mem.h"
#include "vfs.h"
#include "fs/csse/csse.h"
#include "lib/math.h"


int vfs_init() {
  int i;
  mountCount = 0;
  KDS
  for(i = 0; i < MAX_FOPEN; i++) {
    openFileTable[i].open = 0x00;
  }
  SDS
  csse_init();
} 

/*
 * Mounts a drive at a path with a filesystem type.
 * 
 * drive: The index of the drive to mount
 * path: The node path to mount the drive at. 
 *    Must be a buffer of size >= 256
 * fsType: The type of filesystem to mount
 * */
int mountfs(char drive, char * path, int fsType) {
  int len;
  KDS
  
  if (mountCount == MAX_MOUNTS)
    return -1; /* Too many mounts. */
    
  /* Populate a new entry in the mountTable */
  mountTable[mountCount].drive = drive;
  memcpySK(mountTable[mountCount].path, path, 256);
  mountTable[mountCount].fsType = fsType;
  
  /* Verify path length and add trailing '/' */
  len = strlen(mountTable[mountCount].path);
  if (len >= 255){
    SDS
    return -1; /* Too large of a path. */
  }
  if (mountTable[mountCount].path[len-1] != '/') {
    if (len > 254){
      SDS
      return -1; /* No room to add trailing '/' */
    }
    mountTable[mountCount].path[len] = '/';
    mountTable[mountCount].path[len+1] = '\0';
  }

  mountCount++;

  /* Eventually we will have init by filesystem type */
  SDS
  return 0;
}

/*
 * Finds the mount responsible for the resource at abspath
 * assumes absPath and relPath are stack pointers
 * 
 * absPath: An absolute path to a resource.
 * relPath: A buffer to return the relative path to a resource.
 * 
 * returns: The mountIndex of the responsible mount, -1 if not found.
 * */
int findMountAndRelPath(char * absPath, char  * relPath) {
  int maxMatched = 0, mountIndex = -1, i;
  char tmp;
  KDS
  
  /* Find the mount point and relative path. */
  for (i = 0; i < mountCount; i++){
    int matched, j;
    for (j = 0; j < 256; j++) {
      KDS
      if (mountTable[i].path[j] == '\0') {
        matched = j;
        break;
      }
      tmp = mountTable[i].path[j];
      SDS
      if (absPath[j] != tmp) {
        matched = 0;
        break;
      }
    }
    if (matched > maxMatched) {
      maxMatched = matched;
      mountIndex = i;
    }
    KDS
  }
  SDS
  
  if (maxMatched == 0) {
    /* Could not find mount point. */
    return -1;
  }

  /* Copy relative segment of file path. We also include the leading / of its relative path.*/
  memcpy(relPath, absPath+(maxMatched-1), 256-(maxMatched-1));
  
  return mountIndex;
}

/*
 * Reads the filenames from a directory into a buffer. The buffer should 
 * be 65536 (256*256) bytes.
 * 
 * path: The path of the directory to read.
 * buffer: The buffer to write the results to.
 * 
 * returns: Number of entries read, -1 if failed.
 * */
int freaddir(char * path, char * buffer) {
  int mountIndex, fsType, drive;
  char relPath[256];
  
  mountIndex = findMountAndRelPath(path, relPath);
  if (mountIndex < 0) {
    return -1;  /* Could not find mount. */
  }
  
  KDS
  fsType = mountTable[mountIndex].fsType;
  drive = mountTable[mountIndex].drive;
  SDS
  
  if (fsType == FS_CSSE)
    return csse_listFilesInDir(drive, relPath, buffer);
  return -1; /* Invalid fs type */
}

/*
 * Opens a file at a specific path.
 * 
 * path: The node path to open.
 * mode: The mode ('r' or 'w') to open the file with.
 * */
int fopen(char * path, char mode) {
  int res, openFileIndex = 0, mountIndex, fsType;
  char relPath[256];
    
  /* Find open file slot in table. */
  while (openFileIndex < MAX_FOPEN) {
    KDS
    if (!openFileTable[openFileIndex].open)
      break;
    openFileIndex++;
  }
  SDS
  if (openFileIndex == MAX_FOPEN) {
    return -1; /* Too many files open. */
  }
  
  mountIndex = findMountAndRelPath(path, relPath);
  memcpySK(openFileTable[openFileIndex].relPath, relPath, 256);
  if (mountIndex < 0)
    return -1; /* Could not find mount. */
  
  /* Copy parameters into slot. */
  KDS
  openFileTable[openFileIndex].readWriteIndex = 0;
  openFileTable[openFileIndex].mode = mode;
  openFileTable[openFileIndex].mountIndex = mountIndex;
  fsType = mountTable[mountIndex].fsType;
  SDS
  
  if (fsType == FS_CSSE){
    res = csse_fopen(openFileIndex);
    if (res < 0) 
      return -1; /* Problem. */
  }
  
  /* Mark file as open and be done! */
  KDS
  openFileTable[openFileIndex].open = 0xFF;
  SDS
  return openFileIndex;
}

/*
 * Reads count bytes from a file into a buffer
 * 
 * */
int fread(int openFileIndex, char * buffer, int count) {
  int fsType;
  
  if (openFileIndex < 0 || openFileIndex > MAX_FOPEN){
    return -1; /* Invalid openFileIndex. */
  }
  KDS
  fsType = mountTable[openFileTable[openFileIndex].mountIndex].fsType;
  SDS
  
  if (fsType == FS_CSSE)
    return csse_fread(openFileIndex, buffer, count);
  return -1;
}

/*
 * Writes count bytes from a buffer into a file
 * 
 * */
int fwrite(int openFileIndex, char * buffer, int count) {
  int mountIndex, fsType;
  char mode;
  
  if (openFileIndex < 0 || openFileIndex > MAX_FOPEN){
    return -1; /* Invalid openFileIndex. */
  }
  KDS
  mode = openFileTable[openFileIndex].mode;
  SDS
  
  if (mode != 'w')
    return -1; /* Must have write permissions. */
  
  KDS
  fsType = mountTable[openFileTable[openFileIndex].mountIndex].fsType;
  SDS
  
  if (fsType == FS_CSSE)
    return csse_fwrite(openFileIndex, buffer, count);
    
  return -1;
}

int fjump(int openFileIndex, int readWriteIndex) {
  KDS
  openFileTable[openFileIndex].readWriteIndex = readWriteIndex;
  SDS
}

int fdel(char * path) {
  int res, mountIndex;
  char relPath[256];

  mountIndex = findMountAndRelPath(path, relPath);
  if (mountIndex < 0)
    return -1; /* Could not find mount. */
  KDS
  if (mountTable[mountIndex].fsType == FS_CSSE){
    SDS
    csse_fdel(mountTable[mountIndex].drive, relPath);
  }
  SDS
}

/*
 * Closes a specific file index.
 * 
 * */
int fclose(int openFileIndex) {
  int fsType;
  
  if (openFileIndex < 0 || openFileIndex > MAX_FOPEN){
    return -1; /* Invalid openFileIndex. */
  }
  KDS
  fsType = mountTable[openFileTable[openFileIndex].mountIndex].fsType;
  SDS
  if (fsType == FS_CSSE)
    csse_fclose(openFileIndex);
    
  KDS
  openFileTable[openFileIndex].open = 0x00;  
  SDS
  return 0;
}
