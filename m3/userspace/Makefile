# Milestone 3 userspace/Makefile
# Team 3-C
# David Mehl, Christian Balcom, and Alexander Hirschfeld

CFLAGS = -ansi -c

## Collect all files into the build directory, they will be inserted 
## into the filesystem by compileOS.sh
userspace: var/message.txt bin/tstprg bin/tstpr2
	mkdir -p build
	cp var/message.txt build
	cp bin/tstprg build
	cp bin/tstpr2 build

## Remove temporary userspace
clean: 
	rm -rf build
