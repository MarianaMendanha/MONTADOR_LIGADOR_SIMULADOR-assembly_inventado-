mod_1: begin

section data   
n: extern       
mod_2: extern   
public finish   
public divide   
result: space   ;15
section text   
input n         ;0
input n         ;2
jmp mod_2       ;4
divide: load n  ;6
div n           ;8
store result    ;10
output result   ;12
finish: stop    ;14
end