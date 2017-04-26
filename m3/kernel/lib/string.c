/*
  Milestone 3 kernel/lib/string.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */

#include "string.h"

char * strcat(char * a, char * b) {
  char * tmp = a;
  while (*a != '\0') {
    a++;
  }
  while (*b != '\0') {
    *a = *b;
    a++;
    b++;
  }
  *a = '\0';
  return tmp;
}

char * strchr(char *s, char c) {
  while (*s != c) {
    if (*s == '\0')
      return s;
    s++;
  }
  return s;
}

void * memcpy(void *dest, void *src, int count) {
  char *tmp = dest;
  char *s = src;

  while (count--)
    *tmp++ = *s++;
  return dest;
}

void * memset(void *s, int c, int count){
  char *xs = s;

  while (count--)
    *xs++ = c;
  return s;
}
