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
    movq $5, %rax
    movq %rax, -40(%rbp)
    movq $14, %rax
    movq %rax, -24(%rbp)
    movq $25, %rax
    movq %rax, -32(%rbp)
    movq -40(%rbp), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    call printf
    movq -24(%rbp), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    call printf
    movq -32(%rbp), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    call printf
    movq $0, %rax
    movq %rbp, %rsp
    popq %rbp
    ret

