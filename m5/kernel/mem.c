#include "mem.h"

void memcpyKS(char *dest, char *src, int count) {
  char tmp;
  int ds = getDataSegment();

  while (count--){
    KDS
    tmp = *src++;
    SDS
    *dest++ = tmp;
  }
  setDataSegment(ds);
}

void memcpySK(char *dest, char *src, int count) {
  char tmp;
  int ds = getDataSegment();

  while (count--){
    SDS
    tmp = *src++;
    KDS
    *dest++ = tmp;
  }
  setDataSegment(ds);
}
