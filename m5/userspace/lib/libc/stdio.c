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

void exec(char *fname, int shouldWait) {
  interrupt(0x21, 4, fname, shouldWait, 0);
}

void exit() {
  interrupt(0x21, 11, 0, 0, 0);
}

void kill(int proc) {
  interrupt(0x21, 10, proc, 0, 0);
}


void fread(char *fname, char *buf) {
  interrupt(0x21, 3, fname, buf, 0);
}

void fwrite(char *fname, char *buf) {
  interrupt(0x21, 8, fname, buf, 0);
}

void fdel(char *fname) {
  interrupt(0x21, 7, fname, 0, 0);
}

void getArgv(char * buffer) {
  interrupt(0x21, 20, buffer, 0);
}

int __mkargv(int a, int b, int c, int d) {
  puts(a);
  puts(b);
  puts(c);
  puts(d);
  while(1);
}
