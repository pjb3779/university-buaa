.data
A: .word 1, 2, 3, 4, 5, 6, 7, 8, 9, 10  # 배열 A 초기화

.text
.globl main

main:
    # 초기값 설정
    li $t0, 8               # f = 8
    li $t1, 2               # x = 2
    la $t2, A               # $t2에 배열 A의 시작 주소 로드

    # 1. t1 = x * 4
    sll $t1, $t1, 2         # $t1 = $t1 << 2 (x를 2비트 왼쪽 시프트)

    # 2. t3 = A + t1
    add $t3, $t2, $t1       # $t3 = $t2 + $t1 (A의 시작 주소 + x * 4)

    # 3. t3 = A[x+1]
    lw $t3, 4($t3)          # $t3 = 메모리[$t3 + 4] (A[x+1] 값 로드)

    # 4. t3 = t3 + f
    add $t3, $t3, $t0       # $t3 = $t3 + $t0 (A[x+1] + f)

    # 5. t3 = t3 * 2
    add $t3, $t3, $t3       # $t3 = $t3 + $t3 (2 * (A[x+1] + f))

    # 6. f = t3
    add $t0, $t3, $zero     # $t0 = $t3 (결과 저장)

    li $v0, 1               # 시스템 호출 코드 1 (정수 출력)
    move $a0, $t0           # $t0 값을 $a0로 이동
    syscall
	
    # 프로그램 종료
    li $v0, 10              # 종료 시스템 호출
    syscall
