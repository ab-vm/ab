%include "Ab/State.nasm"

extern ab_act

global ab_interpret_func

section .text

; Byte* ab_interpret_func(ExecState*, ExecAction)
ab_interpret_func:
    push rbx
    mov rbx, rdi
    call ab_act
    cmp byte [rbx + ExecState.st_b + ExecStateB.condition], ExecCond_HALTED
    je ab_exit
    int 3
ab_exit:
    pop rbx
    ret
