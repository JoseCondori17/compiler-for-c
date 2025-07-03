.section .rodata
str_format: .string "%s\n"
int_format: .string "%d\n"
float_format: .string "%.2f\n"
char_format: .string "%c\n"
ptr_format: .string "%p\n"

.text
.global main


# Función: main()
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $48, %rsp # espacio para variables locales
    # Reserva espacio para variable 'ciudad' en offset -8
    movq $1, %rax
    movq %rax, -8(%rbp)
    movq $0, %rax
    movq %rax, 0(%rbp)
    movq $0, %rax
    movq %rax, 8(%rbp)
    movq $30, %rax
    movq %rax, 16(%rbp)
    movq $45, %rax
    movq %rax, 24(%rbp)
    movq -8(%rbp), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    movq $0, %rax
    call printf
    movq 0(%rbp), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    movq $0, %rax
    call printf
    movq 8(%rbp), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    movq $0, %rax
    call printf
    movq 16(%rbp), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    movq $0, %rax
    call printf
    movq 24(%rbp), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    movq $0, %rax
    call printf
    movq $0, %rax
    movq %rax, -8(%rbp)
    movq $1, %rax
    movq %rax, 8(%rbp)
    movq 24(%rbp), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    movq $0, %rax
    call printf
    movq $0, %rax
    movq %rbp, %rsp
    popq %rbp
    ret
