# CS 61C Su13 Homework 3
# Name:Fei Liu
# Login:cs61c-ew
	
	.data
mem: 	.space 80 # 4 bytes/word * 20 words
testAB: .asciiz "Testing linked lists A and B (should be 0): "
testAC: .asciiz "Testing linked lists A and C (should be 1): "
testAD:	.asciiz "Testing linked lists A and D (should be 0): "
nl: 	.asciiz "\n"

# Linked lists for testing:
# A: 1,0,2,2,9,5,NULL
# B: 1,0,2,2,9,1,NULL
# C: 2,9,0,1,5,2,NULL
# D: 1,0,2,2,9,NULL
A5:	.word 5	0
A4: .word 9 A5
A3: .word 2 A4
A2: .word 2 A3
A1: .word 0 A2
A0: .word 1 A1

B5:	.word 1 0
B4:	.word 9 B5
B3:	.word 2 B4
B2:	.word 2 B3
B1:	.word 0 B2
B0:	.word 1 B1

C5:	.word 2 0
C4:	.word 5 C5
C3:	.word 1 C4
C2:	.word 0 C3
C1:	.word 9 C2
C0:	.word 2 C1

D4:	.word 9 0
D3:	.word 2 D4
D2:	.word 2 D3
D1:	.word 0 D2
D0:	.word 1 D1

	.text
	.globl main 

# Function that sets the values of a block of memory to zero.
# Do NOT modify this function.
# Params:
#  $a0 = beginning of block of memory
#  $a1 = number of words to set to zero
zeroMem:
	beq $a1,$zero,done
	sw $zero,0($a0)
	addiu $a0,$a0,4
	addi $a1,$a1,-1
	j zeroMem
done:
	jr $ra
	
# Function that traverses through a linked list whose values contain
# the numbers 0 to 9 and stores the count of each number into an array.
# Params:
#  $a0 = beginning of the linked list
#  $a1 = a pointer to memory for the array
#	You may assume that the memory block contains enough space for 10 elements

countList:
# YOUR CODE HERE

	addi $sp,$sp,-12
	sw   $ra,8($sp)
	sw   $a0,4($sp)
	sw   $a1,0($sp)
#start some thing for func

	
LOOPcountList:	
	lw  $t1,0($a0)		# save the value of the node

	sll $t2,$t1,2		# multiply by 4 to get byte-wise shift amount
	add $t3,$a1,$t2		# find the right location for count array for this node
	lw  $t4,0($t3)		# save the nth count to $t4
	addi $t4,$t4,1		# increment it by one
	sw   $t4,0($t3)		# put it back
	
	lw  $t5,4($a0)		# read the next node

	beq $t5,$zero,ENDcountList	# jump if null ended
	move $a0,$t5
	#addi $t3,$t3,1		# increment for one step for count array
	
	j LOOPcountList
	
ENDcountList:
	
	lw   $ra,8($sp)
	lw   $a0,4($sp)
	lw   $a1,0($sp)
	addi $sp,$sp,12

	jr   $ra

# Function that checks if the numbers (0-9) stored in two linked lists are 
# anagrams of each other (ie. if two lists contains the same number 
# of each int). Each linked list will be at least 1 element long.
# You should first traverse each linked list and store the count of each digit into
# an array (memory for it is given to you), and then check if the two arrays
# are equal.
# Params:
#  $a0 = first element of linked list 1
#  $a1 = first element of linked list 2
#  $a2 = memory segment containing space for 2 arrays of 10 ints each.
#	 Note that the memory segment may contain garbage values.
#
# Return (put in $v0): 1 if linked lists are anagrams, 0 if not

isAnagram:
# YOUR CODE HERE

# Perhaps prepare the block of memory somehow?

	addi $sp,$sp,-16
	sw   $ra,12($sp)
	sw   $a0,8($sp)
	sw   $a1,4($sp)
	sw   $a2,0($sp)
# prepare to pass in	
	move $a0,$a2
	addi $a1,$zero,20
# zero out the memory
	jal  zeroMem 
	lw   $ra,12($sp)
	lw   $a0,8($sp)
	lw   $a1,4($sp)
	lw   $a2,0($sp)

# Counts first list
	move $a1,$a2

	jal countList
	lw   $ra,12($sp)
	lw   $a0,8($sp)
	lw   $a1,4($sp)
	lw   $a2,0($sp)
# Counts second list
	move $a0,$a1
	addi $a2,$a2,40
	move $a1,$a2
	
	jal countList
	lw   $ra,12($sp)
	lw   $a0,8($sp)
	lw   $a1,4($sp)
	lw   $a2,0($sp)
# Check arrays for equality
	add $t0,$zero,$zero
	addi $t6,$zero,10
LOOPARRAY:
	beq $t0,$t6,Equal
	sll $t1,$t0,2
	add $t2,$t1,$a2
	lw  $t3,0($t2)
	addi $t4,$t2,40
	lw  $t5,0($t4)
	bne $t3,$t5,NOTEqual
	addi $t0,$t0,1
	j LOOPARRAY
	
NOTEqual: addi $v0,$zero,0
	 j ENDisAnagram	
Equal: addi $v0,$zero,1
	 j ENDisAnagram		
	
ENDisAnagram:
	
	sw   $ra,12($sp)
	sw   $a0,8($sp)
	sw   $a1,4($sp)
	sw   $a2,0($sp)
	addi $sp,$sp,16
	jr $ra

# Program starts here. Do NOT modify anything below this line.
main:
	la $a0,testAB	# Test LL A and B
	li $v0,4
	syscall
	la $a0,A0
	la $a1,B0
	la $a2,mem
	jal isAnagram
	move $a0,$v0
	li $v0,1
	syscall
	la $a0,nl
	li $v0,4
	syscall 
	
	la $a0,testAC	# Test LL A and C
	li $v0,4
	syscall
	la $a0,A0
	la $a1,C0
	la $a2,mem
	jal isAnagram
	move $a0,$v0
	li $v0,1
	syscall
	la $a0,nl
	li $v0,4
	syscall

	la $a0,testAD	# Test LL A and D
	li $v0,4
	syscall
	la $a0,A0
	la $a1,D0
	la $a2,mem
	jal isAnagram
	move $a0,$v0
	li $v0,1
	syscall
	li $v0 10	# Exit
	syscall
