global _start
_start:
    mov rax, 21
    push rax
    mov rax, 100
    push rax
    mov rax, 100000
    push rax
    mov rax, 60
    push QWORD [rsp + 16]
    pop rdi
    syscall
