;.686P
;.MODEL FLAT, C
;.STACK 1000h

.CODE
bughunt_syscall PROC

    ; RCX -> arg1
    ; RDX -> arg2
    ; R8  -> arg3
    ; R9  -> arg4

    push rbp ; prologue
    mov rbp, rsp
    sub rsp, 118h

    mov rax, rcx ;
    mov r10, rdx
    mov rdx, r8
    mov r8, r9

    ; mov rcx, [rbp + XXh] ; main (argv[X + 4])
    ; push rcx

    mov rcx, [rbp + 110h] ; main (argv[28 + 4]) = dw0x1B
    push rcx

    mov rcx, [rbp + 108h] ; main (argv[28 + 4]) = dw0x1B
    push rcx

    mov rcx, [rbp + 100h] ; main (argv[27 + 4]) = dw0x1A
    push rcx

    mov rcx, [rbp + 0F8h] ; main (argv[26 + 4]) = dw0x19
    push rcx

    mov rcx, [rbp + 0F0h] ; main (argv[25 + 4]) = dw0x18
    push rcx
    mov rcx, [rbp + 0E8h] ; main (argv[24 + 4]) = dw0x17
    push rcx
    mov rcx, [rbp + 0E0h] ; main (argv[23 + 4]) = dw0x16
    push rcx
    mov rcx, [rbp + 0D8h] ; main (argv[22 + 4]) = dw0x15
    push rcx
    mov rcx, [rbp + 0D0h] ; main (argv[21 + 4]) = dw0x14
    push rcx
    mov rcx, [rbp + 0C8h] ; main (argv[20 + 4]) = dw0x13
    push rcx
    mov rcx, [rbp + 0C0h] ; main (argv[19 + 4]) = dw0x12
    push rcx
    mov rcx, [rbp + 0B8h] ; main (argv[18 + 4]) = dw0x11
    push rcx
    mov rcx, [rbp + 0B0h] ; main (argv[17 + 4]) = dw0x10
    push rcx
    mov rcx, [rbp + 0A8h] ; main (argv[16 + 4])
    push rcx
    mov rcx, [rbp + 0A0h] ; main (argv[15 + 4])
    push rcx
    mov rcx, [rbp + 98h] ; main (argv[14 + 4])
    push rcx
    mov rcx, [rbp + 90h] ; main (argv[13 + 4])
    push rcx
    mov rcx, [rbp + 88h] ; main (argv[12 + 4])
    push rcx
    mov rcx, [rbp + 80h] ; main (argv[11 + 4])
    push rcx
    mov rcx, [rbp + 78h] ; main (argv[10 + 4])
    push rcx 
    mov rcx, [rbp + 70h] ; main (argv[9 + 4])
    push rcx
    mov rcx, [rbp + 68h] ; main (argv[8 + 4])
    push rcx
    mov rcx, [rbp + 60h] ; main (argv[7 + 4])
    push rcx
    mov rcx, [rbp + 58h] ; main (argv[6 + 4])
    push rcx
    mov rcx, [rbp + 50h] ; main (argv[5 + 4])
    push rcx
    mov rcx, [rbp + 48h] ; main (argv[4 + 4])
    push rcx
    mov rcx, [rbp + 40h] ; main (argv[3 + 4])
    push rcx
    mov rcx, [rbp + 38h] ; main (argv[2 + 4])
    push rcx    
    
    mov r9, [rbp + 30h]
    mov rcx, r10

    ; R9  <- main (argv[4])
    ; R8  <- main (argv[3])
    ; RDX <- main (argv[2])
    ; RCX <- main (argv[1])

    syscall ; invoke syscall

    mov rsp, rbp ; epilogue, either that or `leave'
    pop rbp    
    ret

bughunt_syscall ENDP

END
