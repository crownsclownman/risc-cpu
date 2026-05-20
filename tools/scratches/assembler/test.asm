    addi r3, r0, 7936
    sll  r3, r3, 16
    lw   r2, 4(r3)
    andi r2, r2, 1
    beq  r2, r0, -3
    lw   r2, 4(r3)
    andi r2, r2, 1
    beq  r2, r0, -3
    halt
