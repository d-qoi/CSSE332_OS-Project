#!/bin/bash
# Milestone 3 build/compileOS.sh
# Team 3-C
# David Mehl, Christian Balcom, and Alexander Hirschfeld

echo Starting...
cd `dirname $0`
project="`pwd`/.."
echo $project

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

echo Building Userspace
cd $project/userspace
make

echo Copying userspace to floppya.img
cd $project/build
for filename in $project/userspace/build/*; do
  ./tools/loadFile "$filename" $(basename $filename)
done

echo Cleaning up...
cd $project/kernel
make clean
cd $project/build/tools
make clean
cd $project/userspace
make clean
cd $project/bootloader
rm bootload

echo Done
