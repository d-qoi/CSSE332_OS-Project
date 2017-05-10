/*
  userspace/src/shell.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */
#include <stdio.h>

int main() {
	char argv[256];
	puts("\n\r\0");
	getArgs(argv);
	puts(argv);
	puts("\n\r\0");
	exit();
}