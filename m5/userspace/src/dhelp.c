/*
  userspace/src/dhelp.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
*/
#include <stdio.h>

int main() {
	int i, j, k;

	for (i = 0; i<25565; i++)
		for (j = 0; j<25565; j++);

  puts("Help!, Please see the README for a complete list of commands! \n\n\r");
  exit();
}