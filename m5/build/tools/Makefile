# Milestone 3 build/tools/Makefile
# Team 3-C
# David Mehl, Christian Balcom, and Alexander Hirschfeld

CC = gcc

CFLAGS = -Wall -c -g

## A list of options to pass to the linker. 
LDFLAGS = -Wall -g -lm

## Build the program from the object files (-o option)
loadFile: loadFile.o
	$(CC) loadFile.o -o loadFile $(LDFLAGS)

loadFile.o: loadFile.c
	$(CC) $(CFLAGS) loadFile.c -o loadFile.o

## Remove all the compilation and debugging files
clean: 
	rm -f loadFile loadFile.o
