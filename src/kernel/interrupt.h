#define IDT_LOCATION 0x500

extern void interrupt();

void setupInterrupts(short codeOffset);

struct IDTDescr GenIDTDescr(int offset, short selector, char type_attr);

struct __attribute__((packed)) IDTDescr 
{
    short offset_1;
    short selector;
    char zero;
    char type_attr;
    short offset_2;
};

struct __attribute__((packed)) idt_record 
{
    short limit;
    int base;
};