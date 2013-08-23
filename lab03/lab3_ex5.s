          .data
source:   .word   3, 1, 4, 1, 5, 9, 0
dest:     .word   0, 0, 0, 0, 0, 0, 0
countmsg: .asciiz " values copied. "

          .text

main:   la      $a0,source
        la      $a1,dest
        # $v0 is not initialized
	addiu	$v0,$zero,0
	lw     $v1, 0($a0)     # read next word from source
loop:   
	
 
            # increment count words copied
        #add     $a1,$v1,$zero    # write to destination
        sw     $v1,($a1)
        # int is size 4
        addiu   $a0, $a0, 4     # advance pointer to next source
        addiu   $a1, $a1, 4     # advance pointer to next dest
        lw     $v1, 0($a0)     # read next word from source
        addiu   $v0, $v0, 1
        bne     $v1, $zero, loop# loop if word copied not zero

loopend:
        move    $a0,$v0        # $a0 <- count
        jal     puti            # print it

        la      $a0,countmsg    # $a0 <- countmsg
        jal     puts            # print it

        li      $a0,0x0A        # $a0 <- '\n'
        jal     putc            # print it

finish:
        li      $v0, 10         # Exit the program
        syscall




### The following functions do syscalls in order to print data (integer, string, character)
#Note: argument $a0 to syscall has already been set by the CALLEE

puti:
        li      $v0, 1          # specify Print Integer service
        syscall                 # Print it
        jr      $ra             # Return

puts:
        li      $v0, 4          # specify Print String service
        syscall                 # Print it
        jr      $ra             # Return

putc:
        li      $v0, 11         # specify Print Character service
        syscall                 # Print it
        jr      $ra             # Return
