#define VIDEO_MEM 0xb8000
#define COL 80
#define ROW 25

#define TEXT_COLOUR 0x0f

void writeChar(char c, int col, int row);

void screenShift()
{
    char * video_mem = (char *)VIDEO_MEM;

    for(int i = 0; i < ROW - 1; i++)
    {
        for(int j = 0; j < COL; j++)
        {
            *(video_mem + 2 * (COL * i + j) + 1) = *(video_mem + 2 * (COL * (i + 1) + j) + 1);
            *(video_mem + 2 * (COL * i + j) + 2) = *(video_mem + 2 * (COL * (i + 1) + j) + 2);
        }
    }
    for(int j = 0; j < COL; j++)
    {
        *(video_mem + 2 * (COL * (ROW - 1) + j) + 1) = ' ';
        *(video_mem + 2 * (COL * (ROW - 1) + j) + 2) = TEXT_COLOUR;
    }
}

int main() 
{
    char * video_mem = (char *)VIDEO_MEM;

    for(int i = 0; i < COL * ROW; i++)
    {
        *(video_mem + 2 * i + 1) = ' ';
        *(video_mem + 2 * i + 2) = TEXT_COLOUR;
    }

    *video_mem = '"';

    //writeChar('>', 0, 0);
    //writeChar('>', 1, 0);
    //writeChar('>', 2, 0);

    return 0;
}

void writeChar(char c, int col, int row)
{
    int location = VIDEO_MEM + (2 * (COL * row + col));
    char *pointer = (char *)location;
    *pointer = c;
}