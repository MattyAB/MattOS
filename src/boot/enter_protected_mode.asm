[extern main]

enter_protected_mode:
    cli ; Suspend interrupts

    lgdt [gdt_descriptor] ; Hi CPU, this is where our GDT descriptor is.

    ; To make the switch, we set the first bit of the cr0 control register.
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    ; Now issue a far jump (to another segment) to ensure that the CPU clears out its pipeline
    ; before continuing execution in protected mode.
    jmp CODE_SEG:init_pm

[bits 32]
init_pm:

    ; All our old segments are meaningless, so we point our seg. registers to the data seg from our GDT
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000 ; stack position so it's at the top of free space
    mov esp, ebp

    mov ebx, protmodetext
    call print_string_pm

    ; Store code and data gdt location
    mov ax, gdt_start
    push ax
    mov ax, CODE_SEG
    push ax
    mov ax, DATA_SEG
    push ax
    
    call 0x9000 ; Go to kernel_entry program that runs our 'main' function.

    jmp $