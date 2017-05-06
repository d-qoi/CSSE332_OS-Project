/*
  Milestone 3 userspace/lib/libc/string.h
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */

#ifndef _LIB_STRING
#define _LIB_STRING

char * strcat(char *, char *);
char * strchr(char *, char);
int strncmp(char * a, char * b, int max);
void * memcpy(void *, const void *, int count);
void * memset(void *s, int c, int count);
void removeLinefeed(char * s);
int strlen(char *s);

#endif
