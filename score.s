.global score
score:
    mov r2, #0
    cmp r1, r2
    beq .reset
    bgt .incr
.reset:
    mov r0, #0
    mov pc, lr
.incr:
    add r0, r0, #1
    mov pc, lr
