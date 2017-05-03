/*
  Milestone 3 kernel/kernel.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */

#include "interrupt.h"
#include "fs/csse/csse.h"
#include "vfs.h"
#include "io.h"

int main() {
  char buffer[CSSE_MAX_FSIZE];  /* this is the maximum size of a file */
  int f, res;
  char * message = "This is a very interesting message!\n\r";
  println("Initializing Kernel.");

  makeInterrupt21();
  
  vfs_init();
  
  println("Mounting /");
  mountfs(0, "/", FS_CSSE);
  println("Opening /var/messag");
  f = fopen("/var/messag", 'w');
  
  println("Reading from file!");
  fread(f, buffer, 512);
  printString(buffer);

  println("Writing into file!");
  fjump(f, 512);
  fwrite(f, message, strlen(message)+1);
  
  println("Closing file!");
  fclose(f);
  println("File closed!");
  while(1);
  
  println("File read and print demo:");
  interrupt(0x21, 3, "/var/messag", buffer, 0);
  interrupt(0x21, 0, buffer, 0, 0);
  
  /*
  println("Starting shell:");
  interrupt(0x21, 4, "/bin/shell", 0x2000, 0); */
  
}
