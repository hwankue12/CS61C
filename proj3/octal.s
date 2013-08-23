OCTAL: 
       addi $r0,$r0, 0x0 # set 0
       sw   $r3, 2($r0)  # save to reload
       lw   $r1, 0($r0)  # load the 16 bits
       andi  $r2, $r1, 0x7 # load lower 3 bits
       andi  $r3, $r1, 0x38 # load middle 3 bits
       addi $r0, $r0, 0x1  # prepare to shift
       sllv $r3, $r3, $r0  # shift 1 bit left to fit a hex
       or   $r2, $r3, $r2  # combine the first values
       addi $r0, $r0, 0x5  # ready to shift
       srlv $r1, $r1, $r0  # move upper 3 bits to bottom
       andi  $r3, $r1, 0x7  # load upper 3 bits
       addi $r0, $r0, 0x2   # prepare to shift
       sllv $r3, $r3, $r0   # shift left 8 bits
       or   $r2, $r3, $r2   # put everything in one
       disp $r2, 0
       addi $r0,$r0, -8     # reset 0
       lw   $r3,2($r0)      # recall the register
       jr   $r3 
