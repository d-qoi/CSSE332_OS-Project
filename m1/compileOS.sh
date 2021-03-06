#!/bin/bash
# Milestone 1 script
# David Mehl, Christian Balcom, and Alexander Hirschfeld
echo Starting...
echo Assembling bootload.asm
nasm bootload.asm
echo Completed assembling bootload.asm
echo Creating empty floppya.img
dd if=/dev/zero of=floppya.img bs=512 count=2880
echo Finished creaing empty floppya.img
echo Copying bootloader to floppya.img
dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc
echo Completed Milestone 1 Part 4
echo Starting Milestone 1 Part 6
bcc -ansi -c -o kernel.o kernel.c
as86 kernel.asm -o kernel_asm.o
ld86 -o kernel -d kernel.o kernel_asm.o
echo Completed compiling, assembling, and linking
echo Starting to copy to floppya.img
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3
echo All good
echo Cleaning up...
rm kernel
rm kernel.o
rm kernel_asm.o
rm bootload
echo Done

