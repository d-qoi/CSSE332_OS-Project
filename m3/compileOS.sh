#!/bin/bash
# Milestone 3 script
# David Mehl, Christian Balcom, and Alexander Hirschfeld
# Team 3-C
echo Starting...

echo Assembling bootload.asm
nasm bootload.asm
echo Completed assembling bootload.asm

echo Compiling kernel
bcc -ansi -c -o kernel.o kernel.c
bcc -ansi -c -o interrupt.o interrupt.c
bcc -ansi -c -o math.o math.c
as86 kernel.asm -o kernel_asm.o
ld86 -o kernel -d kernel.o kernel_asm.o interrupt.o math.o
echo Completed compiling, assembling, and linking

echo Creating empty floppya.img
dd if=/dev/zero of=floppya.img bs=512 count=2880

echo Copying bootloader to floppya.img
dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc
echo Copying fs map to floppya.img
dd if=map.img of=floppya.img bs=512 count=1 seek=1 conv=notrunc
echo Copying fs dir to floppya.img
dd if=dir.img of=floppya.img bs=512 count=1 seek=2 conv=notrunc
echo Copying kernel to floppya.img
dd if=kernel of=floppya.img bs=512 count=1 seek=2 conv=notrunc

echo Adding message.txt to filesystem.
./loadFile message.txt

dd if=message.txt of=floppya.img bs=512 count=1 seek=30 conv=notrunc
echo All good

echo Cleaning up...
rm kernel
rm kernel.o
rm interrupt.o
rm math.o
rm kernel_asm.o
rm bootload
echo Done
