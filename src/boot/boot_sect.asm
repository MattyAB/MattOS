[org 0x7c00] ; this program is loaded at 0x7c00
[bits 16]
    mov [BOOT_DRIVE], dl ; BIOS stores our boot drive in dl so let's save that for now.

    mov bx, 0
    mov es, bx ; initialize segment registers?

    mov bp, 0x8000 ; set stack base at 0x8000 so it won't overwrite us.
    mov sp, bp ; stack empty for now

    mov si, introtext ; Load our text in.
    call print_string

    call new_line

    mov dh, 0x10 ; read 16 sectors

    mov si, diskreadintro
    call print_string ; Puts the hex string for sector numbers into diskreadintro
    call print_hex ; Now print the hex value

    call new_line

    mov dl, [BOOT_DRIVE] ; drive 0
    mov bx, 0x9000 ; Load our sector to 0x0000:0x9000

    call read_disk

    mov si, diskreadtext
    call print_string
    mov dx, [bx] ; Puts the hex string loaded in
    call print_hex
    ;add bx, 2
    ;mov dx, [bx] ; Puts the hex string loaded in
    ;call print_hex
    ;add bx, 2
    ;mov dx, [bx] ; Puts the hex string loaded in
    ;call print_hex

    mov dx, gdt_start
    call print_hex

    jmp enter_protected_mode

%include "./print/print_string.asm"
%include "print/print_string_vmem.asm"
%include "print/hex_string.asm"
%include "print/new_line.asm"
%include "read_disk_sect.asm"

introtext: db "Loaded 16 bit mode!", 0
diskreadintro: db "Reading ax ", 0
diskreadtext: db "Read, first word ", 0
protmodetext: db "Now in 32 bit protected mode. Calling kernel", 0

BOOT_DRIVE: db 0

%include "enter_protected_mode.asm"
%include "gdt.asm"
%include "print/print_string_pm.asm"

times 510-($-$$) db 0

dw 0xaa55