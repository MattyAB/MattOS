[bits 32]
[org 0x9000]
enter_long_mode:
    mov ebx, long_mode_msg
    call print_string_pm
    call check_cpuid

    ; We have now asserted we can use CPUID, so let's use it to check first whether the extended function is available:
    mov eax, 0x80000000
    cpuid
    cmp eax, 0x80000001
    jl no_long_mode

    ; We have now asserted that we have extended function, so NOW we can use extended function to try and load in long mode!
    mov eax, 0x80000001
    cpuid
    test edx, 1 << 29 ; The long mode flag is bit 29 in the d register
    jz no_long_mode ; No long mode if this flag is not set.

    ; We have asserted that we can use long mode!
    mov ebx, long_mode_msg
    call print_string_pm
    jmp $

no_long_mode:
    mov ebx, no_long_mode_msg
    call print_string_pm
    jmp $

no_long_mode_msg: db 'No long mode!', 0
long_mode_msg: db 'We have long mode!', 0


%include "check_cpuid.asm"
%include "print/print_string_pm.asm"