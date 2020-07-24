#define IDT_LOCATION 0x500

#define PIC1_COMMAND	0x20
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	0xA0
#define PIC2_DATA	(PIC2+1)
#define PIC_EOI     0x20
#define PIC_READ_IRR    0x0a    /* OCW3 irq ready next CMD read */
#define PIC_READ_ISR    0x0b    /* OCW3 irq service next CMD read */

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