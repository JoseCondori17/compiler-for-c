.section .rodata
str_format: .string "%s\n"
int_format: .string "%d\n"
float_format: .string "%.2f\n"

.text
.global main

printer:
    pushq %rbp
    movq %rsp, %rbp
    movq $4, %rax
    movq %rax, -8(%rbp)
    movq $0, %rax
    movq %rbp, %rsp
    popq %rbp
    ret

suma:
    pushq %rbp
    movq %rsp, %rbp
    movq $1, %rax
    movq %rbp, %rsp
    popq %rbp
    ret
main:
    pushq %rbp
    movq %rsp, %rbp
    movq $5, %rax
    movq %rax, -32(%rbp)
    movq $14, %rax
    movq %rax, -16(%rbp)
    movq $25, %rax
    movq %rax, -24(%rbp)
    movq -32(%rbp), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    call printf
    movq -16(%rbp), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    call printf
    movq -24(%rbp), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    call printf
    movq $4, %rax
    movq %rax, -40(%rbp)
    leaq -40(%rbp), %rax
    movq %rax, -48(%rbp)
    movq $1, %rax
    movq %rax, -8(%rbp)
L0:
    movq -8(%rbp), %rax
    pushq %rax
    movq $10, %rax
    popq %rbx
    cmpq %rax, %rbx
    setle %al
    movzbq %al, %rax
    testq %rax, %rax
    jz L2
    pushq %rcx
    pushq %rdx
    pushq %rsi
    pushq %rdi
    pushq %r8
    pushq %r9
    call suma
    popq %r9
    popq %r8
    popq %rdi
    popq %rsi
    popq %rdx
    popq %rcx
L1:
    movq -8(%rbp), %rax
    pushq %rax
    incq -8(%rbp)
    popq %rax
    jmp L0
L2:
    movq $0, %rax
    movq %rbp, %rsp
    popq %rbp
    ret
