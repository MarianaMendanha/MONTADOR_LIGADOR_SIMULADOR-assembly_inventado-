SECTION DATA
                    load x label:
2label load x
DO*IS: CONST    2  ;
tres      :    ;
  quatro:: ;
  fulaninho: mul quatro
OLD_DATA: 2 const    
NEW_DATA: SPACE 2
TMP_DATA:   SPACE     
SECTION TEXT
      INPUT     OLD_DATA         
LOAD OLD_DATA
L1:  DIV      DOIS
STORE  *NEW_DATA ;ddd
MUL DOIS
STORE TMP_DATA
LOAD OLD_DATA, fulano
SUB TMP_DATA
STORE TMP_DATA
OUTPUT TMP_DATA
label:  COPY     NEW_DATA,OLD_DATA ;daddad
label:  COPY     ,OLD_DATA      
label:  COPY     NEW_DATA,
L: add val
NEW_DATA: SPACE 2
label:  COPY     NEW_DATAOLD_DATA
label:  COPY     NEW_DATA,OL _DATA
label:  COPY     NEW_D TA,OLD_DATA
label:  COPY     2NEW_DATA,OLD_DATA
label:  COPY     NEW_DATA,2OLD_DATA
label:  COPY     NE*W_DATA,OLD_DATA
label:  ChOPY     NEW_DATA,OLD_DATA
LhOAD OLL_DATA
JMPP L1

L:     STOP 
     STOP        
C4BOU