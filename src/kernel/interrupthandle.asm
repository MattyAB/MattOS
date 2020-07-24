extern print_interrupt
global interrupt
align 4

interrupt:

%assign i 0
%rep 256
    pushad
    mov ax, i
    push ax
    jmp handleinterrupt
%assign i i+1
%endrep

handleinterrupt:
    cld

    call print_interrupt

    mov al, 0x20
    out 0x20, al
    pop ax ; Move our parameter back off the stack.

    popad
    iret