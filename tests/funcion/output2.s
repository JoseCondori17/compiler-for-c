.section .rodata
str_format: .string "%s\n"
int_format: .string "%d\n"
float_format: .string "%.2f\n"
char_format: .string "%c\n"
ptr_format: .string "%p\n"

.text
.global main


# Función: algo()
algo:
    pushq %rbp
    movq %rsp, %rbp
    movq $20, %rax
    movq %rbp, %rsp
    popq %rbp
    ret

# Función: main()
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp # espacio para variables locales
    # Reserva espacio para variable 'x' en offset -8
    movq %rsp, %r11
    subq $8, %rsp
    call algo
    movq %rax, -8(%rbp)
    movq -8(%rbp), %rax
    movq %rax, %rsi
    leaq int_format(%rip), %rdi
    movq $0, %rax
    call printf
    movq $0, %rax
    movq %rbp, %rsp
    popq %rbp
    ret
