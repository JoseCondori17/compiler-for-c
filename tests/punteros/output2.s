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
    # Reserva espacio para variable 'a' en offset -8
    movq $18, %rax
    # Inicializa 'a' con valor en %rax
    movq %rax, -8(%rbp) # inicializa a
    # Reserva espacio para variable 'b' en offset -16
    leaq -8(%rbp), %rax
    movq %rax, -16(%rbp)
    # Carga dirección almacenada en 'b'
    movq -16(%rbp), %rax
    # Carga valor apuntado por 'b'
    movq (%rax), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    movq $0, %rax
    call printf
    movq $30, %rax
    movq %rax, -8(%rbp)
    # Carga dirección almacenada en 'b'
    movq -16(%rbp), %rax
    # Carga valor apuntado por 'b'
    movq (%rax), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    movq $0, %rax
    call printf
    movq $0, %rax
    movq %rbp, %rsp
    popq %rbp
    ret
