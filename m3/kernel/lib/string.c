/*
  Milestone 3 kernel/lib/string.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */

#include "string.h"

char * strcat(char * a, char * b) {
  char * tmp = a;
  while (*a != '\0')
    a++;
  while (*b != '\0') {
    *a = *b;
    a++;
    b++;
  }
  *a = '\0';
  return tmp;
}
