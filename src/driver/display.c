#include "display.h"
#include "bus.h"

void clearScreen()
{
    unsigned char *video_mem = (unsigned char *)VIDEO_MEM;

    for(int i = 0; i < COL * ROW; i++)
    {
        *(video_mem + 2 * i) = ' ';
        *(video_mem + 2 * i + 1) = TEXT_COLOUR;
    }
}


void displayChar(char c, int offset)
{
    unsigned char *pointer = (unsigned char *)VIDEO_MEM;
    pointer += offset;
    *pointer = c;
}

// Input is pointer to initial point in memory
// Length is how many bytes from that pointer to print
void printHexLine(void *input, int length)
{
    for(int i = 0; i < length; i++)
    {
        char byte[3];
        byte[2] = 0; // Null-terminated

        char data = *(char *)(input + i);

        char low = (data >> 0) & 0xf;
        if(low >= 16)
            byte[0] = '*';
        else if(low < 10)
            byte[0] = low + '0';
        else
            byte[0] = low + 'a' - 10;

        char high = (data >> 4) & 0xf;
        if(high >= 16)
            byte[1] = '*';
        else if(high < 10)
            byte[1] = high + '0';
        else
            byte[1] = high + 'a' - 10;

        Write(byte);
    }

    WriteLine("");
}

void printHex32(int input, int offset)
{
    for(int i = 7; i >= 0; i--)
    {
        char nibble = input & 0xf;
        int storageoffset = i * 2 + offset;
        if(nibble >= 16)
            displayChar('*', storageoffset);
        else if(nibble < 10)
            displayChar(nibble + '0', storageoffset);
        else
            displayChar(nibble + 'a' - 10, storageoffset);
        
        input = input >> 4;
    }
}

void printHex16(short input, int offset)
{
    for(int i = 3; i >= 0; i--)
    {
        char nibble = input & 0xf;
        int storageoffset = i * 2 + offset;
        if(nibble >= 16)
            displayChar('*', storageoffset);
        else if(nibble < 10)
            displayChar(nibble + '0', storageoffset);
        else
            displayChar(nibble + 'a' - 10, storageoffset);
        
        input = input >> 4;
    }
}

void printHex8(char input, int offset)
{
    for(int i = 1; i >= 0; i--)
    {
        char nibble = input & 0xf;
        int storageoffset = i * 2 + offset;
        if(nibble >= 16)
            displayChar('*', storageoffset);
        else if(nibble < 10)
            displayChar(nibble + '0', storageoffset);
        else
            displayChar(nibble + 'a' - 10, storageoffset);
        
        input = input >> 4;
    }
}

void Write(char *strWrite)
{
    int cursorOffset = getCursorPosition();

    int i = 0; // So that we have scope outside the for loop

    for(i = 0; strWrite[i] != 0; i++)
    {
        int offset = cursorOffset + 2 * i;

        displayChar(strWrite[i], offset);

        if(offset == (ROW * COL - 2))
        {
            screenShift();
            setCursorPosition(COL * (ROW - 1) * 2);

            cursorOffset -= COL * 2;
        }

        //printHex(offset, 1600);
    }

    setCursorPosition(cursorOffset + 2 * i);
}

void WriteLine(char *strWrite)
{
    Write(strWrite);

    int cursor = getCursorPosition();
    int line = cursor / (2 * COL);

    if(cursor % (2 * COL) != 0)
        setCursorPosition(2 * COL * (line + 1));

    if(line == 24)
    {
        screenShift();
        setCursorPosition(2 * COL * (ROW - 1));
    }
}


void screenShift()
{
    unsigned char *video_mem = (unsigned char *)VIDEO_MEM;

    for(int i = 0; i < ROW - 1; i++)
    {
        for(int j = 0; j < COL; j++)
        {
            *(video_mem + 2 * (COL * i + j)) = *(video_mem + 2 * (COL * (i + 1) + j));
            *(video_mem + 2 * (COL * i + j) + 1) = *(video_mem + 2 * (COL * (i + 1) + j) + 1);
        }
    }
    for(int j = 0; j < COL; j++)
    {
        *(video_mem + 2 * (COL * (ROW - 1) + j)) = ' ';
        *(video_mem + 2 * (COL * (ROW - 1) + j) + 1) = TEXT_COLOUR;
    }
}

short getCursorPosition()
{
    // Cursor location specified by 0x0E and 0x0F:
    port_byte_out(0x3D4, 0x0e);
    short high = port_byte_in(0x3D5);
    port_byte_out(0x3D4, 0x0f);
    short low = port_byte_in(0x3D5);

    return 2 * ((high << 8) + low);
}

void setCursorPosition(short offset)
{
    offset /= 2; // Offset given as a byte offset

    char high = (offset >> 8) & 0xff;
    char low = offset & 0xff;

    port_byte_out(0x3D4, 0x0e);
    port_byte_out(0x3D5, high);
    port_byte_out(0x3D4, 0x0f);
    port_byte_out(0x3D5, low);
}