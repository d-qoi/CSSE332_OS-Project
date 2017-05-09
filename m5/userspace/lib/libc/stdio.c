/*
  Milestone 3 userspace/lib/libc/stdio.c
  Group 3-C
  David Mehl, Christian Balcom, and Alexander Hirschfeld
 */

void copyFname(char * fname, char * buff) {
  /* fname is often in the data segment, but we need it in the stack
   * segment in order to pass it to the kernel..
   * */
  int i = 0;
  
  while (*fname != '\0'){
    buff[i++] = *(fname++);
    if (i == 256)
      break;
  }
}

void puts(char *str) {
  /* Str is often in the data segment, but we need it in the stack
   * segment in order to pass it to the kernel. We copy 255 bytes of it
   * (and a terminating null char) at a time and print each chunk.
   * */
  int i = 0;
  char buffer[256];
  char a, b, c, d, e;
  char ** test;
  a = '!';
  b = 'd';
  c = 'n';
  d = 'i';
  e = 'f';
  
  #asm
  mov ax, #1
  push ax
  push ax
  push ax
  push ax
  
  #endasm
  
  while (*str != '\0'){
    buffer[i] = *(str);
    str++;
    i++;
    if (i == 255) { /* Flush buffer. */
      buffer[255] = '\0';
      interrupt(0x21, 0, buffer, 5, 0);
      i = 0;
    }
  }
  buffer[0] = 'q';
  *test = buffer;
  while(1);
  interrupt(0x21, 0, buffer, 0, 0);
}

void gets(char *str) {
  interrupt(0x21, 1, str, 0, 0);
}

void getDirList(char *fname, char *buff) {
  char fnameBuffer[256];
  copyFname(fname, fnameBuffer);
  interrupt(0x21, 9, fnameBuffer, buff, 0);
}

void exec(char *fname, int shouldWait) {
  char fnameBuffer[256];
  copyFname(fname, fnameBuffer);
  interrupt(0x21, 4, fnameBuffer, shouldWait, 0);
}

void exit() {
  interrupt(0x21, 11, 0, 0, 0);
}

void kill(int proc) {
  interrupt(0x21, 10, proc, 0, 0);
}


void fread(char *fname, char *buf) {
  char fnameBuffer[256];
  copyFname(fname, fnameBuffer);
  interrupt(0x21, 3, fnameBuffer, buf, 0);
}

void fwrite(char *fname, char *buf) {
  char fnameBuffer[256];
  copyFname(fname, fnameBuffer);
  interrupt(0x21, 8, fnameBuffer, buf, 0);
}

void fdel(char *fname) {
  char fnameBuffer[256];
  copyFname(fname, fnameBuffer);
  interrupt(0x21, 7, fnameBuffer, 0, 0);
}

int __mkargv(int a, int b, int c, int d) {
  puts(a);
  puts(b);
  puts(c);
  puts(d);
  while(1);
}
