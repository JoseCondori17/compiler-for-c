.section .rodata
str_format: .string "%s\n"
int_format: .string "%d\n"
float_format: .string "%.2f\n"
char_format: .string "%c\n"
ptr_format: .string "%p\n"

.text
.global main


# Función: add(int a, int b)
add:
    pushq %rbp
    movq %rsp, %rbp
    movq %rdi, -8(%rbp) # guarda a
    movq %rsi, -16(%rbp) # guarda b
    movq -8(%rbp), %rax
    pushq %rax
    movq -16(%rbp), %rax
    popq %rbx
    addq %rbx, %rax
    movq %rbp, %rsp
    popq %rbp
    ret

# Función: main()
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp # espacio para variables locales
    # Reserva espacio para variable 'x' en offset -8
    movq $5, %rax
    movq %rax, %rsi
    movq $3, %rax
    movq %rax, %rdi
    movq %rsp, %r11
    subq $8, %rsp
    call add
    # Inicializa 'x' con valor en %rax
    movq %rax, -8(%rbp) # inicializa x
    movq -8(%rbp), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    movq $0, %rax
    call printf
    movq $0, %rax
    movq %rbp, %rsp
    popq %rbp
    ret
