.section .rodata
str_format: .string "%s\n"
int_format: .string "%d\n"
float_format: .string "%.2f\n"

.text
.global main

main:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp
    movq $20, %rax
    movq %rax, -16(%rbp)
    movq $10, %rax
    movq %rax, -24(%rbp)
    movq -16(%rbp), %rax
    pushq %rax
    movq -24(%rbp), %rax
    popq %rbx
    cmpq %rax, %rbx
    setg %al
    movzbq %al, %rax
    testq %rax, %rax
    jz L0
    movq $400, %rax
    movq %rax, -32(%rbp)
L0:
    movq -24(%rbp), %rax
    pushq %rax
    movq -16(%rbp), %rax
    popq %rbx
    cmpq %rax, %rbx
    setl %al
    movzbq %al, %rax
    testq %rax, %rax
    jz L2
    movq -32(%rbp), %rax
    pushq %rax
    movq $100, %rax
    popq %rbx
    addq %rbx, %rax
    movq %rax, -32(%rbp)
L2:
    movq -32(%rbp), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    call printf
    movq $0, %rax
    movq %rbp, %rsp
    popq %rbp
    ret

