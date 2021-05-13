mod_2: begin

section data
finish: extern
mod_1: extern
divide: extern
public mod_2
public n
n: space         ;6
section text     
load n           ;0
jmpz finish      ;2
jmp divide       ;4
end