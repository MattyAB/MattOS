#define VIDEO_MEM 0xb8000

#define COL 80
#define ROW 25
#define TEXT_COLOUR 0x0f


void displayChar(char c, int offset);
void clearScreen();
void printHexLine(void *input, int length);
void printHex32(int input, int offset);
void printHex16(short input, int offset);
void printHex8(char input, int offset);
void Write(char *strWrite);
void WriteLine(char *strWrite);
short getCursorPosition();
void setCursorPosition(short offset);
void screenShift();