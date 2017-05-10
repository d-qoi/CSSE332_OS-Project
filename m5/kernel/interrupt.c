/*
  Milestone 3 kernel/interrupt.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */

#include "io.h"
#include "mem.h"
#include "vfs.h"
#include "fs/csse/csse.h"
#include "interrupt.h"
#include "processing.h"
#include "lib/string.h"
#include "processing.h"

void handleInterrupt21(int ax, int bx, int cx, int dx) {
  int f, bytesRead;
  
  switch (ax) {
    case 0: /* Print *bx as a string */
      printString((char *) bx);
      break;
      
    case 1: /* Read string to *bx */
      readString((char *) bx);
      break;
      
    case 2: /* Read sector cx to bx */
      readSector((char *) bx, cx);
      break;
      
    case 3: /* Read file at *bx into *cx */
      f = fopen((char *) bx, 'r');
      if (f < 0) {
        println("Could not open file.");
        break;
      }
      bytesRead = fread(f, (char *) cx, CSSE_MAX_FSIZE);
      fclose(f);
      break;
  
    case 4:
      /* Execute program at filename *bx, args *cx*/
      /*
      println((char *) bx);
      PRINT_CHAR('\n');
      PRINT_CHAR('\r');
      println((char *) cx);
      PRINT_CHAR('\n');
      PRINT_CHAR('\r');
      */
      
      executeProgram((char *) bx, (char *) cx, (int) dx);
      break;

    case 5: /* Terminate current program. */
      terminate(currentProcess);
      break;
      
    case 6: /* Write a sector */
      writeSector((char *) bx, (char *) cx);
      break;

    case 7: /* Delete a file */
      fdel((char *) bx);
      break;

    case 8: /* Write a file */
      f = fopen((char *) bx, 'w');
      fwrite(f, (char *) cx, strlen((char *) cx) + 1);
      fclose(f);
      break;

    case 9:
      freaddir((char *) bx, (char *) cx);
      break;

    case 10:
      terminate(bx);
      break;

    case 11:
      terminate(-1);
      break;
    
    case 15:
      *((int *)dx) = fopen((char *) bx, (char) cx);
      break;
    case 16:
      bytesRead = fread((int) bx, (char *) cx, *((int *) dx));
      *((int *) dx) = bytesRead;
      break;
    case 17:
      *((int *)dx) = fwrite((int) bx, (int) cx);
      break;
    case 18:
      *((int *)dx) = fclose((int) bx);
      break;
    case 19:
      *((int *)dx) = fmkdir((char *) bx);
      break;
      
    
    case 20:
      passArgsOfCurrent(bx);      
      break;

    case 21:
      PRINT_CHAR(currentProcess + 0x30);
      break;
  }
  SDS
}

