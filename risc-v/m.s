#write a loop that add two number repeatedly.
_start:
    addi x1, x0, 2
    addi x2, x0, 5
    addi x3, x0, 0

repeat_additon:
    add x3, x3, x1
    addi x2, x2, -1
    bne x2, x0, repeat_additon

j .    
    