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
    # Reserva espacio para variable 'value' en offset -8
    movq $42, %rax
    # Inicializa 'value' con valor en %rax
    movq %rax, -8(%rbp) # inicializa value
    # Reserva espacio para variable 'ptr' en offset -16
    leaq -8(%rbp), %rax
    # Inicializa 'ptr' con valor en %rax
    movq %rax, -16(%rbp) # inicializa ptr
    # Reserva espacio para variable 'dbl_ptr' en offset -24
    leaq -16(%rbp), %rax
    # Inicializa 'dbl_ptr' con valor en %rax
    movq %rax, -24(%rbp) # inicializa dbl_ptr
    movq $0, %rax
    movq %rbp, %rsp
    popq %rbp
    ret
