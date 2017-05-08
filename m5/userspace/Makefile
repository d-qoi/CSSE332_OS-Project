# Milestone 3 userspace/Makefile
# Team 3-C
# David Mehl, Christian Balcom, and Alexander Hirschfeld

CFLAGS = -ansi -c -Ilib/libc

all: bin/shell bin/type # bin/pargs

bin/type: src/type.o lib/libc/stdio.o lib/libc/string.o lib/lib_asm.o
	ld86 -o bin/type -d src/type.o lib/libc/stdio.o lib/libc/string.o lib/lib_asm.o

src/type.o: src/type.c
	bcc $(CFLAGS) -o src/type.o src/type.c

bin/shell: src/shell.o lib/libc/stdio.o lib/libc/string.o lib/lib_asm.o
	ld86 -o bin/shell -d src/shell.o lib/libc/stdio.o lib/libc/string.o lib/lib_asm.o

src/shell.o: src/shell.c
	bcc $(CFLAGS) -o src/shell.o src/shell.c

bin/pargs: src/pargs.o lib/libc/stdio.o
	ld86 -o $@ -d $^

src/pargs.o: src/pargs.c
#	bcc $(CFLAGS) -o $@ $^

lib/libc/stdio.o: lib/libc/stdio.c
	bcc $(CFLAGS) -o lib/libc/stdio.o lib/libc/stdio.c

lib/libc/string.o: lib/libc/string.c
	bcc $(CFLAGS) -o lib/libc/string.o lib/libc/string.c

lib/lib_asm.o: lib/lib.asm
	as86 lib/lib.asm -o lib/lib_asm.o

clean: 
	rm -rf lib/libc/*.o src/*.o bin/shell bin/type bin/pargs
