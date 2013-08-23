main:
	lui	$a0,0x8000	# should be 31
	jal	first1pos
	jal	printv0
	lui	$a0,0x0001	# should be 16
	jal	first1pos
	jal	printv0
	li	$a0,1		# should be 0
	jal	first1pos
	jal	printv0
	add	$a0,$0,$0
	jal	first1pos
	jal	printv0
	li	$v0,10
	syscall


first1pos:	# placeholder to call different versions: first1posshift of first1posmask
	addiu	$sp, $sp, -4
	sw	$ra, 0($sp)
	jal	first1posshift
#	jal first1posmask
	lw	$ra, 0($sp)
	addiu	$sp, $sp, 4
	jr	$ra
##################

first1posshift:
	addi	$sp,$sp,-4
	sw	$s0,0($sp)
	addi 	$s0,$zero,31
LOOPfirst:
	beq	$a0,$zero,returnMinus1
	slt	$t0,$a0,$zero
	bne	$t0,$zero,finishFirst
	addi	$s0,$s0,-1
	sll	$a0,$a0,1
	j LOOPfirst	
returnMinus1:
	li	$v0,-1
	lw	$s0,0($sp)
	addi	$sp,$sp,4
	jr 	$ra
finishFirst:
	move 	$v0,$s0
	lw	$s0,0($sp)
	addi	$sp,$sp,4
	jr 	$ra
#################

first1posmask:
		addi	$sp,$sp,-8
	sw	$s0,0($sp)
	sw	$s1,4($sp)
	addi 	$s0,$zero,31
	li	$s1,0x80000000
LOOPSecond:
	li	$t0,0
	beq	$a0,$zero,returnMinus1Again
	and	$t0,$a0,$s1
	bne	$t0,$zero,finishSecond
	addi	$s0,$s0,-1
	srl	$s1,$s1,1
	j LOOPSecond	
returnMinus1Again:
	li	$v0,-1
	lw	$s0,0($sp)
	lw	$s1,4($sp)
	addi	$sp,$sp,8
	jr 	$ra
finishSecond:
	move 	$v0,$s0
	lw	$s0,0($sp)
	lw	$s1,4($sp)
	addi	$sp,$sp,8
	jr 	$ra
#################
printv0:
	addi	$sp,$sp,-4
	sw	$ra,0($sp)
	add	$a0,$v0,$0
	li	$v0,1
	syscall
	li	$v0,11
	li	$a0,'\n'
	syscall
	lw	$ra,0($sp)
	addi	$sp,$sp,4
	jr	$ra
