; load DH sectors to ES:BX from drive DL
read_disk:
    push dx ; save this so we know how many sectors we requested

    ; Running with int 13h
    mov ah, 0x02 ; Read sectors from drive

    ;mov dl, 0 ; drive 0
    mov ch, 0 ; cylinder 0
    mov al, dh ; DH sectors
    mov dh, 0 ; head 0 (based 0)
    mov cl, 2 ; sector 2 (based 1), this is the first sector after our boot sector

    int 0x13 ; carry out the interrupt

    pop dx

    jc disk_error ; jumps if cf is set

    cmp al, dh ; should have read DH
    jne disk_error

    ret


disk_error:
    cmp ah, 0x0c ; Is this a 'Media type not found' error?
    jne disk_print_error

    cmp al, 0
    je disk_print_error

    ; We've got to this point so have confirmed the media type not found error, and that SOME segments were read
    ; So let's try to continue into our kernel!
    ret

disk_print_error:
    mov si, disk_error_msg
    call print_string

    mov al, 0x0
    mov dx, ax
    call print_hex

    jmp $

disk_error_msg:
    db 'Disk read error!', 0