.section .rodata
str_format: .string "%s\n"
int_format: .string "%d\n"
float_format: .string "%.2f\n"

.text
.global main

suma:
    pushq %rbp
    movq %rsp, %rbp
    subq $0, %rsp
    movq 8(%rbp), %rax
    pushq %rax
    movq 16(%rbp), %rax
    popq %rbx
    addq %rbx, %rax
    movq %rbp, %rsp
    popq %rbp
    ret

main:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
    movq $10, %rax
    movq %rax, -16(%rbp)
    movq $20, %rax
    movq %rax, -24(%rbp)
    pushq %rax
    pushq %rcx
    pushq %rdx
    pushq %rsi
    pushq %rdi
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    movq -16(%rbp), %rax
    movq %rax, %rdi
    movq -24(%rbp), %rax
    movq %rax, %rsi
    call suma
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rdi
    popq %rsi
    popq %rdx
    popq %rcx
    popq %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    call printf
    movq $0, %rax
    movq %rbp, %rsp
    popq %rbp
    ret

