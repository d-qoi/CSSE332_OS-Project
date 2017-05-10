

#ifndef _MEM_H_
#define _MEM_H_

#define KDS setKernelDataSegment();
#define SDS setStackDataSegment();

void memcpyKS(char * dest, char * src, int count);
void memcpySK(char * dest, char * src, int count);

#endif
