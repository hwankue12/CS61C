	.data
	# This shows you can use a .word and directly encode the value in hex or use .float
#num1:	.word 0x40800000	#CHANGE ME
num1:	.word 0x4B800000	#CHANGE ME
num2:	.float 1.0
num3:	.word 0xCB000000	#CHANGE ME
result:	.word 0
newl: 	.asciiz "\n"
str1:	.asciiz "num1: "
str2:	.asciiz "num2: "
str3:	.asciiz "num3: "
str4:	.asciiz "sum1: "
str5:	.asciiz	"sum2: "
streq:	.asciiz " = "
pt1:	.asciiz "** PART 1 **"
pt2:	.asciiz "** PART 2 **"

	.text
main:
	la	$t0, num1	#load address of first floating number
	lwc1	$f2, 0($t0)	#load num1 into a fp register
	la	$t0, num2
	lwc1	$f4, 0($t0)	#load num2 into a fp register
	la	$t0, num3
	lwc1	$f6, 0($t0)	#load num3 into a fp register


	# PART 1 - add num1 to num2 and should result in num1
	la	$a0, pt1
	jal	printStr
	jal	printNewline
	
	# display the numbers
	mov.s	$f12, $f2	# num1
	la	$a0, str1
	jal	printStr
	jal	printVals
	
	mov.s	$f12, $f4	# num2
	la	$a0, str2
	jal	printStr
	jal	printVals
	jal	printNewline

	# sum1 = num1 + num2 
	add.s	$f12, $f2, $f4
	la	$a0, str4
	jal	printStr
	jal	printVals
	jal	printNewline
	
	
	# PART 2 - add num1, num2, num3 in 2 different orders to get 2 different results:
	la	$a0, pt2
	jal	printStr
	jal	printNewline
	
	# display the numbers
	mov.s	$f12, $f2	# num1
	la	$a0, str1
	jal	printStr
	jal	printVals
	
	mov.s	$f12, $f4	# num2
	la	$a0, str2
	jal	printStr
	jal	printVals

	mov.s	$f12, $f6	# num3
	la	$a0, str3
	jal	printStr
	jal	printVals
	jal	printNewline
	
	# sum1 = num1 + num2 + num3
	add.s	$f12, $f2, $f4
	add.s	$f12, $f6, $f12
	la	$a0, str4
	jal	printStr
	jal 	printVals
	
	# sum2 = num3 + num2 + num1
	add.s	$f12, $f6, $f4
	add.s	$f12, $f2, $f12
	la	$a0, str5
	jal	printStr
	jal	printVals
	
	# exit
	li	$v0, 10
	syscall


printVals: # $f12 holds the float
	addiu	$sp, $sp, -4
	sw	$ra, 0($sp)

	la	$t0,  result	
	swc1	$f12, 0($t0)	# move the value from floating point reg to mem
	lw	$a0, 0($t0)	# read the value into a regular register
	jal	printHex	# print (hex)
	la	$a0, streq
	jal	printStr
	jal	printFlt	# print (float) - reads from $f12 instead of $a0
	jal	printNewline
	
	lw	$ra, 0($sp)
	addiu	$sp, $sp, 4
	jr	$ra
		
printNewline:
	li	$v0, 4
	la	$a0, newl
	syscall
	jr	$ra

printStr:
	li	$v0, 4
	syscall
	jr	$ra

printHex:
	li	$v0, 34
	syscall
	jr	$ra
	
printFlt:
	li	$v0, 2
	syscall
	jr	$ra
