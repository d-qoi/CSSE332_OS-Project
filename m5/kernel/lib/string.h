/*
  Milestone 4 kernel/lib/string.h
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */

#ifndef _LIB_STRING
#define _LIB_STRING

char * strcat(char *, char *);
char * strncat(char * a, char * b, int max);
char * strchr(const char *, char);
void * memcpy(void *, const void *, int count);
void * memset(void *s, int c, int count);
int strlen(char * s);

#endif
