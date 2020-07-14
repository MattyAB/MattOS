[bits 64]
[extern main]
[global _start]

_start:
    mov ah, 0x0f
    mov al, '%'

    mov edx, 0xb8000
    mov [edx], ax

    call main


    jmp $