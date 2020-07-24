#include "interrupt.h"
#include "../driver/display.h"
#include "../driver/bus.h"

#define INITIAL_STACK 0x90000

int main() 
{
    clearScreen();
    setCursorPosition(0);

    short gdtStart = *(short *)(INITIAL_STACK - 0x2);
    short codeSeg = *(short *)(INITIAL_STACK - 0x4);
    short dataSeg = *(short *)(INITIAL_STACK - 0x6);

    printHexLine((void *)&gdtStart, 2);
    printHexLine((void *)&codeSeg, 2);
    printHexLine((void *)&dataSeg, 2);

    setupInterrupts(codeSeg);

    printHexLine((void *)0x500, 16);

    char string2[] = "Helloiwyaneee";
    WriteLine(string2);

    asm("int $1");

    //printHexLine((void *)0x500, 16);

    return 0;
}

/** TEST CURSOR
 * 
 * 
 * 
   for(int a = 0; a < ROW*COL; a++)
    {
        wait();
        setCursorPosition(a * 2);
        a = getCursorPosition() / 2;
        printHex16(a * 2, 160);
    }

 ** PRINT KERNEL

void wait()
{
    for(int i = 0; i < 0xffffff / 2; i++);
}

void print_kernel(int offset)
{
    int kernel_loc = 0x9000;

    for(int byteno = 0; byteno < 0x200; byteno++)
    {
        char hex = *(char *)(byteno + kernel_loc + offset);

        printHex8(hex, 2 * (byteno));
    }
}

 * 
 * **/