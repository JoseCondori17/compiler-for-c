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
    subq $32, %rsp # espacio para variables locales
    # Reserva espacio para variable 'x' en offset -8
    movq $10, %rax
    # Inicializa 'x' con valor en %rax
    movq %rax, -8(%rbp) # inicializa x
    # Reserva espacio para variable 'y' en offset -16
    movq $20, %rax
    # Inicializa 'y' con valor en %rax
    movq %rax, -16(%rbp) # inicializa y
    # Reserva espacio para variable 'z' en offset -24
    movq $30, %rax
    # Inicializa 'z' con valor en %rax
    movq %rax, -24(%rbp) # inicializa z
    movq -8(%rbp), %rax
    pushq %rax
    movq -16(%rbp), %rax
    popq %rbx
    cmpq %rax, %rbx
    setg %al
    movzbq %al, %rax
    testq %rax, %rax
    jz L0
    movq -8(%rbp), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    movq $0, %rax
    call printf
    movq $0, %rax
    movq %rbp, %rsp
    popq %rbp
    ret
L0:
    movq -8(%rbp), %rax
    pushq %rax
    movq -16(%rbp), %rax
    popq %rbx
    cmpq %rax, %rbx
    setl %al
    movzbq %al, %rax
    testq %rax, %rax
    jz L2
    movq -16(%rbp), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    movq $0, %rax
    call printf
    movq $0, %rax
    movq %rbp, %rsp
    popq %rbp
    ret
L2:
    movq -24(%rbp), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    movq $0, %rax
    call printf
    movq $0, %rax
    movq %rbp, %rsp
    popq %rbp
    ret
