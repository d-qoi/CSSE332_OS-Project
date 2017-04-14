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
