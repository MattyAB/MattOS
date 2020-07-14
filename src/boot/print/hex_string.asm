; Hex value stored in dx
; String storage location stored in si

hex_string:
    pusha

    mov cx, 5 ; current storage offset for 16 bits of hex (4 characters + 0x)

    call hex_digit

hex_loop:
    shr dx, 4 ; Shift next nibble in
    call hex_digit
    
    cmp cx, 1
    jg hex_loop

    popa
    ret


; Has the current nibble stored in least significant nibble of dx
hex_digit:
    push dx
    push si
    
    and dx, 0x000f

    cmp dx, 10
    jge hex_letter ; This is if our value is a letter.

hex_number: ; Else it is a number
    add dl, '0' ; To get the ascii formatted number
    jmp hex_end

hex_letter:
    add dl, ('a' - 10) ; To get the ascii formatted number

hex_end:
    add si, cx ; Add our storage offset
    mov [si], dl

    dec cx

    pop si
    pop dx
    ret

; Prints the hex string stored in dx
print_hex:
    pusha

    mov si, hexstr

    call hex_string
    call print_string

    popa
    ret

hexstr: db "0x0000", 0