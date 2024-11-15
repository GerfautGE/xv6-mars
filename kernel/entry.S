.section .text
.global _entry
_entry:

        # keep hartid in tp
        # hartid is in a0 when we enter the kernel
        # This is given by openSBI
        mv tp, a0

        # set up a stack for C.
        # stack0 is declared in start.c,
        # with a 4096-byte stack per CPU.
        # sp = stack0 + (hartid * 4096)

        la sp, stack0
        li t0, 4096 # Page size is 4KB

        addi t1, a0, 1 # t1 = hartid + 1
        mul t2, t0, t1 # t2 = (hartid + 1) * 4096
        add sp, sp, t2 # sp = stack0 + (hartid * 4096)
        # jump to start() in start.c
        call start

spin:
        wfi
        j spin
