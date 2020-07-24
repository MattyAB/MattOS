#include "interrupt.h"

#include "../driver/display.h"
#include "../driver/bus.h"

short picGetIRQ(int ocw)
{
    port_byte_out(PIC1_COMMAND, ocw);
    port_byte_out(PIC2_COMMAND, ocw);

    return (port_byte_in(PIC2_COMMAND) << 8) | port_byte_in(PIC1_COMMAND);
}

short picGetIRR()
{
    return picGetIRQ(PIC_READ_IRR);
}

short picGetISR()
{
    return picGetIRQ(PIC_READ_ISR);
}

void print_interrupt(char interruptno)
{
    switch(interruptno)
    {
        case 8:
            // This is clock interrupt
            break;
        case 9:
            // Handle keyboard interrupt
            Write("Keyboard interrupt recieved!");
            printHex16(picGetIRR(), 560);
            //port_byte_out(PIC1_COMMAND, PIC_EOI);
            //port_byte_out(PIC2_COMMAND, PIC_EOI);
            //printHex16(port_short_in(PIC1_COMMAND), 560);
            break;
        default:
            Write("Interrupt recieved: ");
            printHex8(interruptno, getCursorPosition());
            WriteLine("");
            break;
    }
}

// *** Interrupt Descriptor Table

void load_idt(struct idt_record* idtr)
{
    asm("lidt %0" :: "m"(*idtr));
}

void setupInterrupts(short codeOffset)
{
    WriteLine("Setting up interrupts...");

    int cpuidval = 0;

    asm ("mov %1, %%eax\n\t"
        "cpuid\n\t"
        "mov %%edx, %0"
        : "=r" (cpuidval) 
        : "r" (0x01));

    cpuidval = cpuidval & (1 << 9);

    if(cpuidval == 1 << 9)
        WriteLine("APIC Present!");
    else
        WriteLine("No APIC!");

    struct IDTDescr *IDT_Pointer = (struct IDTDescr *)IDT_LOCATION;
    struct IDTDescr *IDT_Initial_Pointer = IDT_Pointer;

    for(int i = 0; i < 256; i++)
    {
        int offset = (int)(interrupt + 12 * i);
        // codeOffset is the offset of that segment in the GDT, and 0b01 specifies ring 1, 0 specifies that it is a GDT
        short selector = codeOffset + 0b010; 
        // 1 present, 00 descriptor privelage level, 0 not storage segment, 1110 32-bit interrupt gate
        char type_attr = 0b10001110;
        struct IDTDescr Descriptor = GenIDTDescr(offset, selector, type_attr);

        //*(struct IDTDescr*)(IDT_LOCATION + 8 * i) = Descriptor;
        *IDT_Pointer = Descriptor;
        // Increment it only by one as it is of type struct IDTDescr* ,
        // so knows to increment literal pointer by 8 bytes.
        IDT_Pointer += 1; 
    }


    short sizeValue = IDT_Pointer - IDT_Initial_Pointer - 1;

    struct idt_record idtr;
    idtr.base = (int)IDT_Initial_Pointer;
    idtr.limit = sizeValue;
    printHexLine(&idtr, 6);

    load_idt(&idtr);

    asm("sti"); // Begin interrupts
}

struct IDTDescr GenIDTDescr(int offset, short selector, char type_attr)
{
    struct IDTDescr descriptor;

    short offset_1 = offset & 0xffff;
    short offset_2 = (offset >> 16) & 0xffff;

    descriptor.selector = selector;
    descriptor.type_attr = type_attr;
    descriptor.offset_1 = offset_1;
    descriptor.offset_2 = offset_2;

    return descriptor;
}