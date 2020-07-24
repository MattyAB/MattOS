#include "bus.h"

unsigned char port_byte_in(unsigned short port)
{
    unsigned char result;
    asm("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void port_byte_out(unsigned short port, unsigned char data)
{
    *(char *)0x8b000 = '^';
    asm("out %%al, %%dx" : : "a" (data), "d" (port));
}

unsigned short port_short_in(unsigned short port)
{
    unsigned short result;
    asm("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void port_short_out(unsigned short port, unsigned short data)
{
    *(char *)0x8b000 = '^';
    asm("out %%al, %%dx" : : "a" (data), "d" (port));
}