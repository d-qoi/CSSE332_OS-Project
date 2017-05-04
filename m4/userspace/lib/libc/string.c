/*
  Milestone 3 userspace/lib/libc/string.c
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
      return 0;
    s++;
  }
  return s;
}

int strncmp(char * a, char * b, int max) {
  int count = 0;
  while(count < max) {
    count++;
    if(*a == *b) {
      if (*a == '\0') {
	/* Done here */
	return 0;
      }
      a++;
      b++;
    } else {
      return 1;
    }
  }
  return 0;
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

void removeLinefeed(char * s) {
  int index = 0;
  while(s[index] != '\r') {
    index++;
  }
  s[index] = '\0';
}

int strlen(char *s) {
  int i = 0;
  while (s[i] != '\0')
    i++;
  return i;
}
