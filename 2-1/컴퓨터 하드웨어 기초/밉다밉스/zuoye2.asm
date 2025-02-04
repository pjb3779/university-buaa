.data
	n:	.word 0
	cnt:	.word 0
.text
	main:	
		li $v0, 5
		syscall
		move $t0, $v0	#n
		
		li $t1, 0	#cnt
		li $t2, 2	#i
		li $t3,0
		
	loopi:
		bgt $t2, $t0, endi
		
		div $t0, $t2
		mfhi $t4
		beqz $t4 startj
		j iadd
	startj:
		li $t3, 1
		li $t5, 2
		li $t6, 0
		
		j loopj
	loopj:
		li $t7, 0
		mul $t7, $t5, $t5
		bgt $t7, $t2, endj
		
		div $t2, $t5	#i,j
		mfhi $t7
		
		beqz $t7, true
		
		j addj
	iadd:
		addi $t2, $t2, 1
		li $t3,0 #?ûà?ñ¥?ñó?Çò?Öè?
		j loopi
	addj:
		addi $t5, $t5, 1
		
		j loopj
	true:
		li $t3, 0
		j endj
	endj:	
		beqz $t3, iadd
		while:
			div $t0, $t2
			mfhi $t8
			bnez $t8, addcnt
			mflo $t0
			j while
		j  addcnt
	addcnt:
		addi $t1, $t1, 1
		j iadd
	endi:
		li $v0, 1
		move $a0, $t1
		syscall
		
		
		li $v0, 10
		syscall
		
		
