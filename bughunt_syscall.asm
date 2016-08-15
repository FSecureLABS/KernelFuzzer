.686P
.MODEL FLAT, C
.STACK 1000h

.CODE
bughunt_syscall PROC
    push ebp
    mov ebp, esp
    sub esp, 84h

    mov ecx, [ebp + 88h] ; main (argv[33]) = dw0x20
    push ecx
    mov ecx, [ebp + 84h] ; main (argv[32]) = dw0x1F
    push ecx
    mov ecx, [ebp + 80h] ; main (argv[31]) = dw0x1E
    push ecx
    mov ecx, [ebp + 7Ch] ; main (argv[30]) = dw0x1D
    push ecx
    mov ecx, [ebp + 78h] ; main (argv[29]) = dw0x1C
    push ecx
    mov ecx, [ebp + 74h] ; main (argv[28]) = dw0x1B
    push ecx
    mov ecx, [ebp + 70h] ; main (argv[27]) = dw0x1A
    push ecx
    mov ecx, [ebp + 6Ch] ; main (argv[26]) = dw0x19
    push ecx
    mov ecx, [ebp + 68h] ; main (argv[25]) = dw0x18
    push ecx
    mov ecx, [ebp + 64h] ; main (argv[24]) = dw0x17
    push ecx
    mov ecx, [ebp + 60h] ; main (argv[23]) = dw0x16
    push ecx
    mov ecx, [ebp + 5Ch] ; main (argv[22]) = dw0x15
    push ecx
    mov ecx, [ebp + 58h] ; main (argv[21]) = dw0x14
    push ecx
    mov ecx, [ebp + 54h] ; main (argv[20]) = dw0x13
    push ecx
    mov ecx, [ebp + 50h] ; main (argv[19]) = dw0x12
    push ecx
    mov ecx, [ebp + 4Ch] ; main (argv[18]) = dw0x11
    push ecx
    mov ecx, [ebp + 48h] ; main (argv[17]) = dw0x10
    push ecx
    mov ecx, [ebp + 44h] ; main (argv[16]) = dw0x0F
    push ecx
    mov ecx, [ebp + 40h] ; main (argv[15]) = dw0x0E
    push ecx
    mov ecx, [ebp + 3Ch] ; main (argv[14]) = dw0x0D
    push ecx
    mov ecx, [ebp + 38h] ; main (argv[13]) = dw0x0C
    push ecx
    mov ecx, [ebp + 34h] ; main (argv[12]) = dw0x0B
    push ecx
    mov ecx, [ebp + 30h] ; main (argv[11]) = dw0x0A
    push ecx
    mov ecx, [ebp + 2Ch] ; main (argv[10]) = dw0x09
    push ecx
    mov ecx, [ebp + 28h] ; main (argv[9]) = dw0x08
    push ecx
    mov ecx, [ebp + 24h] ; main (argv[8]) = dw0x07
    push ecx
    mov ecx, [ebp + 20h] ; main (argv[7]) = dw0x06
    push ecx
    mov ecx, [ebp + 1Ch] ; main (argv[6]) = dw0x05
    push ecx
    mov ecx, [ebp + 18h] ; main (argv[5]) = dw0x04
    push ecx
    mov ecx, [ebp + 14h] ; main (argv[4]) = dw0x03
    push ecx
    mov ecx, [ebp + 10h] ; main (argv[3]) = dw0x02
    push ecx
    mov ecx, [ebp + 0Ch] ; main (argv[2]) = dw0x01
    push ecx

    mov eax, [ebp + 08h] ; main (argv[1]) = syscall_uid

    mov edx, 7FFE0300h
    call dword ptr [edx]

    mov esp, ebp
    pop ebp
    ret
bughunt_syscall ENDP

END
