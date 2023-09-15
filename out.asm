global _start
_start:
    mov rax, 20
    push rax
    mov rax, 1
    push rax
    mov rax, 2
    push rax
    pop rbx
    pop rax
    mul qword rbx
    push rax
    mov rax, 1
    push rax
    pop rbx
    pop rax
    sub rax, qword rbx
    push rax
    mov rax, 4
    push rax
    mov rax, 2
    push rax
    pop rbx
    cdq
    pop rax
    idiv qword rbx
    push rax
    pop rax
    pop rbx
    add rax, qword rbx
    push rax
    pop rax
    pop rbx
    add rax, qword rbx
    push rax
    pop rax
    push rax
    push QWORD [rsp + 0]
    mov rax, 100000
    push rax
    pop rax
    push rax
    mov rax, 60
    push QWORD [rsp + 8]
    pop rdi
    syscall
