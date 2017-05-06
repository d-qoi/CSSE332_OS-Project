# Milestone 3 kernel/Makefile
# Team 3-C
# David Mehl, Christian Balcom, and Alexander Hirschfeld

CFLAGS = -ansi -c
LIST = kernel.o asm/kernel_asm.o interrupt.o vfs.o io.o lib/math.o lib/string.o fs/csse/csse.o processing.o

## Build the program from the object files (-o option)
kernel: $(LIST)
	ld86 -o kernel -d $(LIST)

asm/kernel_asm.o: asm/kernel.asm
	as86 asm/kernel.asm -o asm/kernel_asm.o

%.o: %.c
	bcc $(CFLAGS) $< -o $@
	
# kernel.o: kernel.c
# 	bcc $(CFLAGS) kernel.c -o kernel.o
	
# interrupt.o: interrupt.c
# 	bcc $(CFLAGS) interrupt.c -o interrupt.o
	
# io.o: io.c
# 	bcc $(CFLAGS) io.c -o io.o
	
# vfs.o: vfs.c
# 	bcc $(CFLAGS) vfs.c -o vfs.o

# fs/csse/csse.o: fs/csse/csse.c
# 	bcc $(CFLAGS) fs/csse/csse.c -o fs/csse/csse.o
	
# lib/math.o: lib/math.c
# 	bcc $(CFLAGS) lib/math.c -o lib/math.o
	
# lib/string.o: lib/string.c
# 	bcc $(CFLAGS) lib/string.c -o lib/string.o

## Remove all the compilation and debugging files
clean: 
	rm -f kernel $(LIST)