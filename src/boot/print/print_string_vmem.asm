VIDEO_MEM equ 0xb8000
;WHITE_ON_BLACK equ 0b00001111

; prints the null terminated string, starting at location SI, using video memory editing
print_string_vmem:
    pusha
    mov bx, 0xb000 ; offset 
    mov es, bx
    mov bx, 0x8000 ; [es:bx] gives 0xb8000

print_string_vmem_loop:
    mov al, [si]
    mov ah, WHITE_ON_BLACK

    cmp al, 0 ; is this our final char?
    je print_string_vmem_end

    mov [es:bx], ax ; move our character describer into the correct location

    add bx, 2
    add si, 1

    jmp print_string_vmem_loop

print_string_vmem_end:
    popa
    ret