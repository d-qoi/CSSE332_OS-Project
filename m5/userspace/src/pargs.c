/*
  userspace/src/shell.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */
#include <stdio.h>

int main(int argc, char * argv[]) {
	while(argc > 0) {
		puts(argv[argc]);
		argc--;
	}
}