/*
  Milestone 3 kernel/lib/string.h
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */

#ifndef _LIB_STRING
#define _LIB_STRING

char * strcat(char *, char *);
char * strchr(const char *, char);
void * memcpy(void *, const void *, size_t count);
void * memset(void *s, int c, size_t count);

#endif
