%include "Ab/State.nasm"

extern ab_act

global ab_interpret_func

section .text

ab_interpret_func:
    call ab_act
    ret