/*
  Milestone 4 kernel/lib/string.c
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

char * strncat(char * a, char * b, int max) {
  char * tmp = a;
  int count = 0;
  while (*a != '\0') {
    count++;
    a++;
    if (count >= max) return tmp;
  }
  while (*b != '\0') {
    *a = *b;
    a++;
    b++;
    count++;
    if (count >= max) return tmp;
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

int strlen(char *s) {
  int i = 0;
  while (s[i] != '\0')
    i++;
  return i;
}

char strcmp(char * s1, char * s2) {
  while(*s1 != '\0'){
    if (*s1 != *s2)
      return 0;
    s1++;
    s2++;
  }
  return (*s2 == '\0');
}
