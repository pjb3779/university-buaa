.macro address(%des,%column)
	sll %des, %column, 2 
.end_macro

.data
a: .word 500

.text
	li $v0,5
	syscall
	move $s0,$v0 #n
	li $t0,0	#jinwei
	beq $t0,$s0,special

func:
	li $t2,0 #tmp
	li $t3,1 #digit
	li $t4,0 #carry
	
	li $t5,1 #just 1
	address($t6,$t5)
	sw $t5,a($t6) #s[1]=1
	
	li $t0,2 #i
loop1:
	li $t1,1 #j
	loop2:
		address($t5,$t1)
		lw $t6,a($t5) #a[j]
		multu $t6,$t0
		mflo $t6 #a[j]*i
		addu $t2,$t6,$t4 #tmp=a[j]*i+carry
		li $t6,10
		divu $t2,$t6
		mfhi $t6 
		sw $t6,a($t5) #a[j]=tmp%10;
		mflo $t4 #carry=tmp/10;
		addi $t1,$t1,1
		ble $t1,$t3,loop2
		beqz $t4,part_end
	while:
		li $t6,10
		divu $t4,$t6
		mfhi $t5 #carry%10
		address($t7,$t1)
		sw $t5,a($t7) #a[j]=carry%10;
		divu $t4,$t4,10 #carry/=10;
		addi $t1,$t1,1
		bnez $t4,while
part_end:
	subi $t3,$t1,1
	addi $t0,$t0,1
	ble $t0,$s0,loop1
	
	move $t0,$t3 #i=digit
loop3:
	address($t1,$t0)
	lw $a0,a($t1)
	li $v0,1
	syscall #printf("%d",s[i]);
	subi $t0,$t0,1
	bne $t0,0,loop3
end:
	li $v0,10
	syscall #return

special:
	li $a0,1
	li $v0,1
	syscall
