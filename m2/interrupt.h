
#define PRINT_COMMAND 0xE
#define PRINT_CHAR(character) interrupt(0x10, PRINT_COMMAND * 256 + character, 0, 0, 0)


void printString(char * string);
