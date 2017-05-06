#!/bin/bash
# Milestone 3 build/compileOS.sh
# Team 3-C
# David Mehl, Christian Balcom, and Alexander Hirschfeld

echo Starting...
cd `dirname $0`
project="`pwd`/.."

echo Assembling bootloader
cd $project/bootloader
nasm bootload.asm

echo Compiling kernel
cd $project/kernel
make

echo Building Tools
cd $project/build/tools
make

echo Building floppya.img
cd $project/build
rm -f floppya.img
dd if=/dev/zero of=floppya.img bs=512 count=2880
dd if=$project/bootloader/bootload of=floppya.img bs=512 count=1 conv=notrunc
dd if=map.img                      of=floppya.img bs=512 count=1 seek=1 conv=notrunc
dd if=dir.img                      of=floppya.img bs=512 count=1 seek=2 conv=notrunc
dd if=$project/kernel/kernel       of=floppya.img bs=512 seek=3 conv=notrunc

echo Building userspace
cd $project/userspace
make

echo Copying userspace to floppya.img
cd $project/build

for filename in $project/userspace/bin/*; do
  ./tools/loadFile "$filename" "/bin/$(basename $filename)"
done

for filename in $project/userspace/var/*; do
  ./tools/loadFile "$filename" "/var/$(basename $filename)"
done

echo Done
