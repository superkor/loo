global _start
_start:
    mov rax, 20
    push rax
    mov rax, 100
    push rax
    mov rax, 60
    push QWORD [rsp + 0]
    pop rdi
    syscall
