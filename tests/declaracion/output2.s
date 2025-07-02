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
    movq $200, %rax
    movq %rax, -16(%rbp)
    movq $100, %rax
    movq %rax, -24(%rbp)
    movq $0, %rax
    movq %rbp, %rsp
    popq %rbp
    ret

