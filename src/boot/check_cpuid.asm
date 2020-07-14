check_cpuid:
    ; Check if CPUID is supported, by attempting to flip the ID flag (bit 21) in the FLAGS register.
    pushfd
    pop eax ; Pushed the double flags register and popped it to somewhere we can work.

    mov ecx, eax ; For comparisons later.

    xor eax, 1 << 21 ; Flip bit 21.

    push eax
    popfd ; Copy our new eax to the flags reg.

    pushfd
    pop eax ; Pop it back to eax so we can compare

    push ecx
    popfd ; Put the original flags register back

    xor eax, ecx ; Compare eax and ecx to see whether the bit were flipped.
    jz no_cpuid

    ret

no_cpuid:
    mov ebx, no_cpuid_msg
    call print_string_pm
    jmp $

no_cpuid_msg: db 'No CPUID!', 0