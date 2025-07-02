.section .rodata
str_format: .string "%s\n"
int_format: .string "%d\n"
float_format: .string "%.2f\n"

.text
.global main

main:
    pushq %rbp
    movq %rsp, %rbp
    subq $48, %rsp
    movq $1, %rax
    movq %rax, -48(%rbp)
    movq $0, %rax
    movq %rax, -40(%rbp)
    movq $0, %rax
    movq %rax, -32(%rbp)
    movq $30, %rax
    movq %rax, -24(%rbp)
    movq $45, %rax
    movq %rax, -16(%rbp)
    movq -48(%rbp), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    call printf
    movq -40(%rbp), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    call printf
    movq -32(%rbp), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    call printf
    movq -24(%rbp), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    call printf
    movq -16(%rbp), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    call printf
    movq $0, %rax
    movq %rax, -48(%rbp)
    movq $1, %rax
    movq %rax, -32(%rbp)
    movq -16(%rbp), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    call printf
    movq $0, %rax
    movq %rbp, %rsp
    popq %rbp
    ret

