multisimd:        addi $r0, $r0, 0x0 #mem adress
        sw   $r3, 5($r0)   #save the $r3
        lw   $r1, 1($r0)   #load A
        lw   $r2, 2($r0)   #load B
        lw   $r3, 3($r0)   #load C
        addi $r0, $r0, 0x8 # prepare to move to upper 8 bits
        sllv $r1, $r1, $r0 # move to upper bits
        or   $r2, $r2, $r1 # put A B to the same register
        addi $r0, $r0, -8
loop:   add  $r2, $r2, $r2
        addi $r3, $r3, -1
        beq  $r3, $r0 ,loop
        lw   $r3, 5($r0)
        jr   $r3


