#!/bin/bash
# Milestone 3 build/cleanOS.sh
# Team 3-C
# David Mehl, Christian Balcom, and Alexander Hirschfeld


echo Cleaning up...

cd `dirname $0`
project="`pwd`/.."

cd $project/kernel
make clean
cd $project/bootloader
rm bootload
cd $project/userspace
make clean
cd $project/build
rm -f floppya.img
cd $project/build/tools
make clean

echo Done
