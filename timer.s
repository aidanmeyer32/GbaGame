.global timer
timer:
    mov r1, #0
.top:
    mov r2, #99
    cmp r1, r2
    beq .end
    add r1, r1, #1
    b .top
.end:
    mov r0, #1
    mov pc, lr
