Group 3-C
David Mehl, Christian Balcom, and Alexander Hirschfeld

To compile our operating system, run:
./compileOS.sh

If this does not work, you may need to run:
chmod +x compileOS.sh

This tells the machine that this file is executable

The compileOS.sh script will create a floppy disk image
It will be named 'floppya.img'
All other files produced in the process will be removed

To test the image with qemu, run:
qemu-system-i386 -fda floppya.img --boot order=a

The program will print "Hello World" to the console.
Then the program will request user input in the form
of a line of text. Enter text and press enter. The
line you entered will be printed on the next line.

Be aware, readString is protected. You cannot backspace
beyond the starting point, and you cannot enter more
than 76 characters. If you attempt to do the latter,
your line will be cut off and the characters you
have typed so far will be printed on the next line.
If you attempt the former, the cursor will not move and
the kernel will still be waiting for you to input
a line, terminated by the enter key.

Next the program will read message.txt from sector 30
and print the contents to the console on the next line.

Finally, the program calls interrupt 21 and in doing so
is waiting for the user input another line. It will print
the inputted line to the next console line as before, but
there is no prompt for this input.
