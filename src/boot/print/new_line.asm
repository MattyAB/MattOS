new_line:
    pusha

    mov ah, 0x03 ; get cursor position interrupt
    int 0x10

    mov ah, 0x02 ; set cursor position interrupt
    mov dl, 0 ; First row to move to the start of the line
    inc dh ; Increment to next line
    int 0x10

    popa
    ret