/*
  Milestone 3 userspace/lib/libc/stdio.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */

void puts(char *str) {
  interrupt(0x21, 0, str, 0, 0);
}

void gets(char *str) {
  interrupt(0x21, 1, str, 0, 0);
}

void getDirList(char *fname, char *buff) {
  interrupt(0x21, 9, fname, buff, 0);
}

void exec(char *fname, char * args, int waiting) {
  interrupt(0x21, 4, fname, args, waiting);
}

void exit() {
  interrupt(0x21, 11, 0, 0, 0);
}

void kill(int proc) {
  interrupt(0x21, 10, proc, 0, 0);
}


void ffread(char *fname, char *buf) {
  interrupt(0x21, 3, fname, buf, 0);
}

void ffwrite(char *fname, char *buf) {
  interrupt(0x21, 8, fname, buf, 0);
}

void fdel(char *fname) {
  interrupt(0x21, 7, fname, 0, 0);
}

int fmkdir(char * path) {
  int ret;
  interrupt(0x21, 19, path, 0, &ret);
  return ret;
}

int fopen(char * path, char mode) {
  int ret;
  interrupt(0x21, 15, path, mode, &ret);
  return ret;
}

int fread(int fid, char * buffer, int bytes) {
  interrupt(0x21, 16, fid, buffer, &bytes);
  return bytes;
}

int fwrite(int fid, char * buffer, int bytes) {
  interrupt(0x21, 17, fid, buffer, &bytes);
  return bytes;
}

int fclose(int fid) {
  int ret;
  interrupt(0x21, 18, fid, 0, &ret);
  return ret;
}

void getArgs(char * buffer) {
  interrupt(0x21, 20, buffer, 0);
}

void printpid() {
  interrupt(0x21, 21, 0, 0);
}

int __mkargv(int a, int b, int c, int d) {
  puts(a);
  puts(b);
  puts(c);
  puts(d);
  while(1);
}
