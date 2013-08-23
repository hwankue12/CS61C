	lui $r0, 0x33          # 0xb033
        ori $r0, $r0, 0x44     # 0x8044
        lui $r1, 0x33          # 0xb133
        ori $r1, $r1, 0x44     # 0x8544
	lui $r2, 0x55          # 0xb255
        ori $r2, $r2, 0x55     # 0x8288
        lui $r3, 0x66          # 0xb366
        ori $r3, $r3, 0x66     # 0x8766
        addi $r3, $r1, 0x66    # 6766
        addi $r2, $r0, 0x66     # 0x6266
self:   beq $r0, $r1, self     # 0x21ff
