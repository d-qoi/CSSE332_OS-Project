/*
  Milestone 4 kernel/vfs.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */
 
#include "io.h"
#include "vfs.h"
#include "fs/csse/csse.h"
#include "lib/math.h"


int vfs_init() {
  int i;
  mountCount = 0;
  for(i = 0; i < MAX_FOPEN; i++) {
    openFileTable[i].open = 0x00;
  }
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
  char pathBuffer;
  
  if (mountCount == MAX_MOUNTS)
    return -1; /* Too many mounts. */
    
  /* Populate a new entry in the mountTable */
  mountTable[mountCount].drive = drive;
  memcpy(mountTable[mountCount].path, path, 256);
  mountTable[mountCount].fsType = fsType;
  
  /* Verify path length and add trailing '/' */
  len = strlen(mountTable[mountCount].path);
  if (len >= 255)
    return -1; /* Too large of a path. */
  if (mountTable[mountCount].path[len-1] != '/') {
    if (len > 254)
      return -1; /* No room to add trailing '/' */
    mountTable[mountCount].path[len] = '/';
    mountTable[mountCount].path[len+1] = '\0';
  }

  mountCount++;

  /* Eventually we will have init by filesystem type */
  return 0;
}

/*
 * Finds the mount responsible for the resource at abspath
 * 
 * absPath: An absolute path to a resource.
 * relPath: A buffer to return the relative path to a resource.
 * 
 * returns: The mountIndex of the responsible mount, -1 if not found.
 * */
int findMountAndRelPath(char * absPath, char  * relPath) {
  int maxMatched = 0, mountIndex = -1, i;
  
  /* Find the mount point and relative path. */
  for (i = 0; i < mountCount; i++){
    int matched, j;
    for (j = 0; j < 256; j++) {
      if (mountTable[i].path[j] == '\0') {
        matched = j;
        break;
      }
      if (absPath[j] != mountTable[i].path[j]) {
        matched = 0;
        break;
      }
    }
    if (matched > maxMatched) {
      maxMatched = matched;
      mountIndex = i;
    }
  }
  
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
  
  fsType = mountTable[mountIndex].fsType;
  drive = mountTable[mountIndex].drive;
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
  int res, openFileIndex = 0, mountIndex;
  
  /* Find open file slot in table. */
  while (openFileIndex < MAX_FOPEN) {
    if (!openFileTable[openFileIndex].open)
      break;
    openFileIndex++;
  }
  if (openFileIndex == MAX_FOPEN) {
    return -1; /* Too many files open. */
  }
  
  mountIndex = findMountAndRelPath(path, openFileTable[openFileIndex].relPath);
  if (mountIndex < 0)
    return -1; /* Could not find mount. */
  
  /* Copy parameters into slot. */
  openFileTable[openFileIndex].readWriteIndex = 0;
  openFileTable[openFileIndex].mode = mode;
  openFileTable[openFileIndex].mountIndex = mountIndex;
  
  if (mountTable[mountIndex].fsType == FS_CSSE){
    res = csse_fopen(openFileIndex);
    if (res < 0) 
      return -1; /* Problem. */
  }
  
  /* Mark file as open and be done! */
  openFileTable[openFileIndex].open = 0xFF;
  return openFileIndex;
}

/*
 * Reads count bytes from a file into a buffer
 * 
 * */
int fread(int openFileIndex, char * buffer, int count) {
  int mountIndex;
  
  if (openFileIndex < 0 || openFileIndex > MAX_FOPEN){
    return -1; /* Invalid openFileIndex. */
  }
  mountIndex = openFileTable[openFileIndex].mountIndex;
  if (mountTable[mountIndex].fsType == FS_CSSE)
    return csse_fread(openFileIndex, buffer, count);
    
  return -1;
}

/*
 * Writes count bytes from a buffer into a file
 * 
 * */
int fwrite(int openFileIndex, char * buffer, int count) {
  //TODO: add fs specific code
}

/*
 * Closes a specific file index.
 * 
 * */
int fclose(int openFileIndex) {
  int mountIndex;
  
  if (openFileIndex < 0 || openFileIndex > MAX_FOPEN){
    return -1; /* Invalid openFileIndex. */
  }
  mountIndex = openFileTable[openFileIndex].mountIndex;
  if (mountTable[mountIndex].fsType == FS_CSSE)
    csse_fclose(openFileIndex);
    
  openFileTable[openFileIndex].open = 0x00;  
  return 0;
}
