.entry LIST
.define spot = 1
MAIN: mov r3,LIST[strLen]
jmp #-5
mov STR[5],STR[2]
sub r1,r4
L1: inc L3
.entry LOOP
bne LOOP
END: hlt
jmp #spot
.define strLen = 7
STR: .string "hello"
LIST: .data strLen,1,2
.extern L3
