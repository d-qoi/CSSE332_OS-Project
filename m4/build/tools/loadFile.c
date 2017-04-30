//loadFile.c
//Michael Black, 2007
//
//Loads a file into the file system
//This should be compiled with gcc and run outside of the OS

// Modified by Christian Balcom to support a second argument (new filename)
// Modified by Christian Balcom to support creating new directory entries

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void loadDir(char * dir, int sector, FILE* floppy) {
  int i = 0;
  fseek(floppy, 512*sector, SEEK_SET);
	for (i=0; i<512; i++)
		dir[i]=fgetc(floppy);
}

void saveDir(char * dir, int sector, FILE* floppy) {
  int i = 0;
  fseek(floppy, 512*sector, SEEK_SET);
  for (i=0; i<512; i++)
		fputc(dir[i],floppy);
}

int fileExists(char * dir, char * fname) {
  int i, j, fnameSame;
  
  for (i = 0; i < 16; i++) {
    if (dir[i*32] == 0x0)
      continue;
    fnameSame = 1;
    j = 0;
    /* Check if the characters in fname match those in the dir entry.*/
    while (fname[j] != '\0') {
      if (j == 6){
        printf("Filename longer than six characters, invalid!\n");
        exit(1);
      }
      if (dir[i*32+j] != fname[j]) {
        fnameSame = 0;
        break;
      }
      j++;
    }

    /* Check that the remaining characters in the dir entry are zero. */
    while (j < 6) {
      if (dir[i*32+j] != '\0'){
        fnameSame = 0;
        break;
      }
      j++;
    }
    
    if (fnameSame)
      return dir[i*32+6];
  }
  return 0;
}

int mapSector(char * map) {
  int i;
  
  //find a free map entry
  for (i=0; i<256; i++)
    if (map[i]==0)
      break;
  if (i==256){
    printf("Not enough room for file\n");
    exit(1);
  }

  //mark the map entry as taken
  map[i]=0xFF;
  
  return i;
}

int createDirectory(char * dir, char * map, char * fname, FILE * floppy) {
  int i, newSector;
  
  //find a free entry in the directory
	for (i=0; i<512; i=i+0x20)
		if (dir[i]==0)
			break;
	if (i==512)
	{
		printf("Not enough room in directory\n");
		exit(1);
	}
	int dirindex=i;

	//fill the name field with 00s first
	for (i=0; i<6; i++)
		dir[dirindex+i]=0x00;
    
	//copy the name over
	for (i=0; i<6; i++)
	{
		if(fname[i]==0)
			break;
		dir[dirindex+i]=fname[i];
  }
  dirindex=dirindex+6;
  newSector = mapSector(map);
  
  //mark the sector in the directory entry
  dir[dirindex]=newSector;

	//move to the sector and write to it
	fseek(floppy,newSector*512,SEEK_SET);
	for (i=0; i<512; i++)
	{
    fputc(0x0, floppy);	
	}
  
  return newSector;
}

void writeFile(char * dir, char * map, char * fname, FILE * loadFile, FILE * floppy) {
  int i, newSector;
  
  //find a free entry in the directory
	for (i=0; i<512; i=i+0x20)
		if (dir[i]==0)
			break;
	if (i==512)
	{
		printf("Not enough room in directory\n");
		return;
	}
	int dirindex=i;

	//fill the name field with 00s first
	for (i=0; i<6; i++)
		dir[dirindex+i]=0x00;
    
	//copy the name over
	for (i=0; i<6; i++)
	{
		if(fname[i]==0)
			break;
		dir[dirindex+i]=fname[i];
  }

	dirindex=dirindex+6;

	//find free sectors and add them to the file
	int sectcount=0;
	while(!feof(loadFile))
	{
		if (sectcount==26)
		{
			printf("Not enough space in directory entry for file\n");
			return;
		}

    newSector = mapSector(map);

		//mark the sector in the directory entry
		dir[dirindex]=newSector;
		dirindex++;
		sectcount++;

		//move to the sector and write to it
		fseek(floppy,newSector*512,SEEK_SET);
		for (i=0; i<512; i++)
		{
			if (feof(loadFile))
			{
				fputc(0x0, floppy);
				break;
			}
			else
			{
				char c = fgetc(loadFile);
				fputc(c, floppy);
			}	
		}
	}
}

int main(int argc, char* argv[])
{
	int i;
  char * path;

	if (argc<2)
	{
		printf("Specify file name to load\n");
		return 1;
	}
  
  // Set path
  if (argc == 2)
		path = argv[1];
  else
    path = argv[2];

	//open the source file
	FILE* loadFil;
	loadFil=fopen(argv[1],"r");
	if (loadFil==0)
	{
		printf("File not found\n");
		return 1;
	}

	//open the floppy image
	FILE* floppy;
	floppy=fopen("floppya.img","r+");
	if (floppy==0)
	{
		printf("floppya.img not found\n");
		return 1;
	}

	//load the disk map
	char map[512];
	fseek(floppy,512,SEEK_SET);
	for(i=0; i<512; i++)
		map[i]=fgetc(floppy);

	//load the root directory
	char dir[512];
  int dirPos = 2; // sector of directory block on fs
  loadDir(dir, dirPos, floppy);

  char nameSeg[7];
  int newSector, segLen;
  char *segEnd;
  
  while (*path != '\0') {
    memset(nameSeg, 0, 7);
    path = strchr(path, '/') + 1;
    segEnd = strchr(path, '/');
    if (segEnd == NULL)
      segEnd = strchr(path, '\0');
    segLen = segEnd - path;
    if (segLen > 6)
      segLen = 6;
    memcpy(nameSeg, path, segLen);
    
    if (*segEnd != '\0') { // Is this a directory?
      newSector = fileExists(dir, nameSeg);
      if (!newSector) // Create new directory
        newSector = createDirectory(dir, map, nameSeg, floppy);
      
      // Shuffle to new directory
      saveDir(dir, dirPos, floppy);
      dirPos = newSector;
      loadDir(dir, dirPos, floppy);
    }
    else {
      writeFile(dir, map, nameSeg, loadFil, floppy);
    }
    
    path = segEnd;
  }

	//write the map and directory back to the floppy image
  fseek(floppy,512,SEEK_SET);
  for(i=0; i<512; i++)
    fputc(map[i],floppy);
        
  saveDir(dir, dirPos, floppy);

	fclose(floppy);
	fclose(loadFil);
}
