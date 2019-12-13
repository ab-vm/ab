%ifndef OMTALK_VMSTRUCTS_NASM_
%define OMTALK_VMSTRUCTS_NASM_

# EntrySaveArea
struc EntrySaveArea
    .sp: resq 1
    .bp: resq 1
endstruc

%define ExecCond_UNDEFINED 0
%define ExecCond_RUNNING 1
%define ExecCond_HALTED 2
%define ExecCond_TRAPPED 3
%define ExecCond_ERRORED 4

%define ExecAction_CRASH 0
%define ExecAction_INTERPRET 1
%define ExecAction_HALT 2
%define ExecAction_EXIT 3

%endif
