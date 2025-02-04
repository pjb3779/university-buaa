.data
result1: .word 0  
result2: .word 0 

.text
.globl main

main:
    li $a0, 0x58           # 01011000
    jal lowbit            
    sw $a0, result1       

    li $a0, 0xA4           # 10100100
    jal lowbit             
    sw $a0, result2        

    lw $a0, result1        
    li $v0, 1              
    syscall                
    
    li $v0, 11            
    li $a0, 10            
    syscall          

    lw $a0, result2       
    li $v0, 1             
    syscall               

    li $v0, 11           
    li $a0, 10             
    syscall               

    j end                  

lowbit:
    move $t0, $a0        
    negu $t1, $t0         
    and $a0, $t0, $t1    
    jr $ra                

end:
    li $v0, 10
    syscall
