Group 3-C
David Mehl, Christian Balcom, and Alexander Hirschfeld

Please read. The directory structure is not the same
as expected. You must use sub-directories to find the
files to read and execute. Examples are found at the
end of this file


To compile our operating system, run:
./build/compileOS.sh

If this does not work, you may need to run:
chmod +x build/compileOS.sh

This tells the machine that this file is executable

The compileOS.sh script will create a floppy disk image
It will be named 'build/floppya.img'
All files produced in the process can be removed
with:
./build/cleanOS.sh

You might need to run:
chmod +x build/cleanOS.sh

To test the image with qemu, run:
qemu-system-i386 -fda build/floppya.img --boot order=a


The program will print "Starting Shell:" to the console,
and then the shell prompt "# " will appear on the next
line. Commands can be entered at this command prompt.
Execution will begin upon hitting the enter key

Commands Available:
'type': This command will print the contents of the
	desired file to the console. Be sure to
	include the correct directory information
	to access the file. For example:
	'type /var/messag' will print the contents of
	messag in the directory /var/

'execute': This command will execute the desired
	   program. Be sure to include the correct
	   directory information to access the
	   executable. For example:
	   'execute /bin/tstpr2' will execute
	   the program 'tstpr2' in the directory
	   /bin/

'delete': This command will delete the desired file.
	  Be sure to include the correct directory
	  information to access the file.
	  'delete /var/messag'

'copy':   This command will copy the desired file.
	  'copy /var/messag /m' will copy the contents
	  of /var/messag into a new file m in the root
	  directory


'dir':	  This command will list the contents of the
	  directory supplied.
	  'dir /var/' will provide the contents of
	  directory /var/

'create': This command prompts the user to enter text
	  to put in a file with the name they supplied
	  'create /m' followed by the user entering text
	  will create a new file m that has the contents
	  the user enters up until they enter a blank
	  line of text. In this case, m will be in the
	  root directory

If incorrect arguments are supplied, typically the response
of the shell will be 'Invalid arguments to ### command' where
### is the name of the command

Some commands will give an extra bit of information, such as
what argument is incorrect
