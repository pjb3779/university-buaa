.data
	a: .word 0
	b: .word 0

.text
	main:
		li $v0, 5
		syscall
		sw $v0, a
		
		li $v0, 5
		syscall
		sw $v0, b
	
		lw $t0, a
		lw $t1, b
	loop:	
		beqz $t1, end
		
		div $t0, $t1
		mfhi $t2
		move $t0,$t1
		move $t1,$t2
		j loop
		
	end:
		move $a0, $t0
		
		li $v0, 1
		syscall
		
		li $v0, 10
		syscall