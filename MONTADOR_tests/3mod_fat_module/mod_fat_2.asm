mod_fat: begin
section data
mod_fim: extern
aux: extern
n: extern
fim: extern
public one
public mod_fat
ONE: CONST 1
section text
fat: sub ONE
JMPZ fim
STORE AUX
MUL N
STORE N
LOAD auX
JMP MOD_FIM
end