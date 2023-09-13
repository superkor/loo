global _start
_start:
    mov rax, 22
    push rax
    push QWORD [rsp + 0]
    mov rax, 100000
    push rax
    mov rax, 60
    push QWORD [rsp + 8]
    pop rdi
    syscall
