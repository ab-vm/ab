%ifndef OMTALK_VMSTRUCTS_NASM_
%define OMTALK_VMSTRUCTS_NASM_
struc EntrySaveArea
    .sp: resq 1
    .bp: resq 1
endstruc

; ExeCond
%define ExecCond_UNDEFINED 0
%define ExecCond_RUNNING 1
%define ExecCond_HALTED 2
%define ExecCond_TRAPPED 3
%define ExecCond_ERRORED 4

; ExeAction
%define ExecAction_CRASH 0
%define ExecAction_INTERPRET 1
%define ExecAction_HALT 2
%define ExecAction_EXIT 3

struc Flags
    .trap: resb 1
    .error: resb 1
endstruc

struc ExecStateA
    .ip: resq 1
    .sp: resq 1
    .fn: resq 1
endstruc

struc ExecStateB
    .func: resq 1
    .stack: resq 1
    .condition: resb 1
    .flags: resb 1
endstruc

struc ExecState
    .st_a: resb ExecStateA_size
    .st_b: resb ExecStateB_size
endstruc

%endif
