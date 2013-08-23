	.text
main:
	la	$a0,n1
	la	$a1,n2
	jal	proc2	# proc1 crashes swapb, proc2 makes swapb work
	jal	swapb	# swapa works correctly, swapb throws error
	li	$v0,1	# print n1 and n2; should be 27 and 14
	lw	$a0,n1
	syscall
	li	$v0,11
	li	$a0,' '
	syscall
	li	$v0,1
	lw	$a0,n2
	syscall
	li	$v0,11
	li	$a0,'\n'
	syscall
	li	$v0,10	# exit
	syscall

swapa:	addi $sp,$sp,-4
	lw   $t0,0($a0)
	sw   $t0,0($sp)
	lw   $t0,0($a1)
	sw   $t0,0($a0)
	lw   $t0,0($sp)
	sw   $t0,0($a1)
	addiu $sp,$sp,4
	jr $ra 
	
swapb:	addi $sp,$sp,-4
	lw   $t0,0($a0)
	lw   $t3,0($sp) #find what the temp pointer points to 
	sw   $t0,0($t3) #save the a0 to the address the pointer points to
	lw   $t1,0($a1) 
	sw   $t1,0($a0)
	lw   $t2,0($t3)	# dereference from the address temp points to
	sw   $t2,0($a1)
	addiu $sp,$sp,4
	jr $ra 


proc1:	# custom procedure to crash buggy swap
	addi $sp,$sp,-4
	sw   $zero,0($sp)
	addi $sp,$sp,4
	jr   $ra
proc2:	# custom procedure to guarantee buggy swap works
	addi $sp,$sp,-4
	addi $t0,$sp,-4
	sw   $t0,0($sp)
	addi $sp,$sp,4
	jr   $ra
	.data
n1:	.word	14
n2:	.word	27
n3:	.word	0
