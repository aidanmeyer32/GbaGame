.global gameover
gameover:
    mov r1, #10
    cmp r0, r1
    bge .end
    mov r0, #0
    mov pc, lr
.end:
    mov r0, #1
    mov pc, lr
