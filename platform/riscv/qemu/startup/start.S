_start:
    li sp, 0x80002000 # load the stack pointer with a usable stack address. The value that is reachable by the CPU
    jal main # jump an also save the return address. 

j .