SOMA1: BEGIN

section data
test: extern
PUblic start
one:        CONST 1                 ;10
two:        CONST -2                ;11

            section text
start:      add     one              ;0
            jmp     finishing        ;2
store test                           ;4
copy one,test                        ;6

finishing:        stop               ;9
     
END