.section .rodata
str_format: .string "%s\n"
int_format: .string "%d\n"
float_format: .string "%.2f\n"

.text
.global main

main:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
    movq $3, %rax
    movq %rax, -16(%rbp)
    movq $10, %rax
    pushq %rax
    movq -16(%rbp), %rax
    popq %rbx
    subq %rax, %rbx
    movq %rbx, %rax
    movq %rax, -24(%rbp)
    movq -24(%rbp), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    call printf
    movq $0, %rax
    movq %rbp, %rsp
    popq %rbp
    ret

