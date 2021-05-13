mod_main: begin
section data
mod_fat: extern
n: extern
public aux
aux: space
section text
input n
load n
jmp mod_fat
end