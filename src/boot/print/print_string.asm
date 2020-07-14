; prints the null terminated string, starting at location SI
print_string:
    pusha
    mov ah, 0x0e ; Character write function
    mov cx, 1 ; One print of the character
    mov al, [si] ; Move the character in from location specified by si

print_string_loop:
    int 0x10 ; Print the character
    inc si
    mov al, [si] ; Move the character in from location specified by si
    cmp al, 0 ; Is the next character 0?
    jne print_string_loop

    popa
    ret

    mov ah, 0x0e ; Error if we get here
    mov al, '*'
    int 0x10
    jmp $