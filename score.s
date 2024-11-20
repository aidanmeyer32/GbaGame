.global score
score:
.top:
    cmp r0, r1
    bgt .block
    blt .enemy
.block:
    mov r0, #1
    mov pc, lr
.enemy:
    mov r0, #2
    mov pc, lr
