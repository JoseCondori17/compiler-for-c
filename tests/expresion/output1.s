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
    subq $16, %rsp # espacio para variables locales
    # Reserva espacio para variable 'x' en offset -8
    movq $3, %rax
    # Inicializa 'x' con valor en %rax
    movq %rax, -8(%rbp) # inicializa x
    # Reserva espacio para variable 'result' en offset -16
    movq $10, %rax
    pushq %rax
    movq -8(%rbp), %rax
    popq %rbx
    subq %rax, %rbx
    movq %rbx, %rax
    # Inicializa 'result' con valor en %rax
    movq %rax, -16(%rbp) # inicializa result
    movq -16(%rbp), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    movq $0, %rax
    call printf
    movq $0, %rax
    movq %rbp, %rsp
    popq %rbp
    ret
