/*
  userspace/src/shell.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */
#include <stdio.h>

int main() {
	char * argv;
	puts('\0');
	getArgv(argv);
	puts(argv);
	exit();
}