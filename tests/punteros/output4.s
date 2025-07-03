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
    # Reserva espacio para variable 'num' en offset -8
    movq $8, %rax
    # Inicializa 'num' con valor en %rax
    movq %rax, -8(%rbp) # inicializa num
    # Reserva espacio para variable 'ptr' en offset -16
    leaq -8(%rbp), %rax
    # Inicializa 'ptr' con valor en %rax
    movq %rax, -16(%rbp) # inicializa ptr
    movq -8(%rbp), %rax
    pushq %rax
    movq $5, %rax
    popq %rbx
    addq %rbx, %rax
    movq %rax, -8(%rbp)
    movq -8(%rbp), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    movq $0, %rax
    call printf
    # Carga dirección almacenada en 'ptr'
    movq -16(%rbp), %rax
    # Carga valor apuntado por 'ptr'
    movq (%rax), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    movq $0, %rax
    call printf
    movq $0, %rax
    movq %rbp, %rsp
    popq %rbp
    ret
