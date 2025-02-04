.data
buffer: .space 1000
n:      .word 0
x:      .word 0
y:      .word 0

.text
main:
    li $v0, 5
    syscall
    sw $v0, n

    li $v0, 5
    syscall
    sw $v0, x

    li $v0, 5
    syscall
    sw $v0, y

    li $v0, 8	# scan string
    la $a0, buffer	#buffer addr
    li $a1, 1000	#addr size
    syscall

    lw $t0, n
    lw $t1, x
    blt $t1, $t0, valid_x	#x < n
    li $t1, 0		
valid_x:
    sw $t1, x

    lw $t1, y
    blt $t1, $t0, valid_y	#y < n
    move $t1, $t0	#Y = N
valid_y:
    sw $t1, y

    lw $t0, y
    lw $t1, x
    sub $t2, $t0, $t1	#y - x store

    la $a0, buffer
    add $a0, $a0, $t1	#a0 + x
    add $a1, $a0, $t2	#a0 + y - x
    jal reverse_string

    li $v0, 4
    la $a0, buffer
    syscall

    li $v0, 10
    syscall

reverse_string:
    move $t3, $a0	#buffer start
    move $t4, $a1	#buffer end

reverse_loop:
    bge $t3, $t4, reverse_done

    lb $t5, 0($t3)	#read 1bit
    lb $t6, 0($t4)

    sb $t6, 0($t3)	#store 1bit
    sb $t5, 0($t4)

    addi $t3, $t3, 1
    addi $t4, $t4, -1
    j reverse_loop

reverse_done:
    jr $ra
