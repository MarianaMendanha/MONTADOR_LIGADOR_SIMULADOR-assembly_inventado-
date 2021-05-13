#include <iostream>
#include <iomanip>
#include <math.h>
#include <cstring>
#include <climits>
#include <algorithm>
#include <bitset>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <ctype.h>

#define MAX 50

using namespace std;

bool getFileContent(string arq, vector<string> &linhas){
    ifstream fin(arq.c_str());

    if(!fin){
        cerr << "Cannot open the File : "<<arq<<endl;
        return false;
    }
    string str;

    // Read the next line from File untill it reaches the end.
    while(getline(fin, str)){
        // Line contains string of length > 0 then save it in vector
        if(str!=arq){linhas.push_back(str);}
    }

    fin.close();
return true;
}

bool exe(string arq, vector<string> linhas, int module){
    
    //CRIACAO DE TABELA COM COMANDOS
    struct LINHA {   
        string label;
        string instruction;
        string op1;
        string op2;
    }; 
    //VARIAVEIS DO EXE
    vector< pair< string,int > > instructions = {
        {"ADD",1},     
        {"SUB",2},     
        {"MUL",3},     
        {"DIV",4},     
        {"JMP",5},     
        {"JMPN",6},   
        {"JMPP",7},    
        {"JMPZ",8},   
        {"COPY",9},   
        {"LOAD",10},   
        {"STORE",11}, 
        {"INPUT",12}, 
        {"OUTPUT",13}, 
        {"STOP",14},   
        {"SPACE",0},  
        {"CONST",0},
        {"BEGIN",0},
        {"EXTERN",0},
        {"PUBLIC",0},
        {"END",0},  
    };
    int counter=0;
    string linha,tokens_linha,tokens,instr,inst,operandos,op1,op2,label;
    vector<string> semantica_labels,semantica_operandos; 
    vector<pair<string,int>> label_linha,operandos_linha;
    //DECLARAÇÃO DE VARIÁVEIS PARA MÓDULOS
    vector< pair< string,int > > tabUso,tabUso_check,tabDef;

    vector<LINHA> LINE;

    bool diretiva,ehLabel,ehoperando=false,ehComent,inv_dgt;
    bool error_label,error,error_program=false;
    bool label_enter,enter=false,passou=false;

    int addr,j=0,k=0,ADR=0,IT=0,ENT=0;

    for(addr=0; addr<linhas.size(); addr++){

        diretiva=false,ehLabel=false,error_label=false,ehComent=false,inv_dgt=false,label_enter=false,error=false,enter=false;
        int invalid_digit=0,num=0;
        linha = linhas[addr];
        tokens_linha.clear();
        j=0;

        
        //LINHA MAIUSCULA
        for(auto p : linha){tokens_linha.push_back(toupper(p));}

        //PRINT ENDEREÇO E LINHA 
        //cout<<endl<<addr<<"-["<<tokens_linha<<"]-instrucao"<<endl;

        //RETIRADA DE COMENTARIOS
        int comentario = tokens_linha.find(";");
        if(comentario != -1){
            tokens_linha=tokens_linha.substr(0,comentario);
        }
    
        //RETIRADA DE ESPACOS NAS PONTAS DAS INSTRUCOES
        int first=0,last=tokens_linha.length()-1;
        if((tokens_linha[first]==' ' || tokens_linha[first]=='\t') || (tokens_linha[last]==' ' || tokens_linha[last]=='\t')){
            if(tokens_linha[first]==' ' || tokens_linha[first]=='\t'){
                while(tokens_linha[first]==' ' || tokens_linha[first]=='\t'){first++;}
                tokens_linha=tokens_linha.substr(first,tokens_linha.length()-1);
            }
            last=tokens_linha.length()-1;    
            if(tokens_linha[last]==' ' || tokens_linha[last]=='\t'){
                while(tokens_linha[last]==' ' || tokens_linha[last]=='\t'){last--;}
                tokens_linha=tokens_linha.substr(0,last+1);
            }
        }
        
        if(tokens_linha!="\0" && tokens_linha!="\r\n" && tokens_linha!="\n"){
            
            if(tokens_linha == "SECTION DATA" || tokens_linha == "SECTION TEXT"){}   
            else{  

             //TRATAMENTO DO PRIMEIRO CARACTERE DA LINHA
                if((isalpha(tokens_linha[0])) || tokens_linha[0]=='_' || tokens_linha[0]=='\0'){}
                else{error_label=true;error=true;cout<<"linha "<<addr+1<<":: "<<"erro lexico: primeiro caractere da linha invalido ->["<<tokens_linha<<"]"<<endl;}
             //RÓTULO||DIRETIVA
                int rotulo=tokens_linha.find_last_of(':');
             
              //EHLABEL   
                if(rotulo!=-1){     
                    tokens=tokens_linha.substr(0,rotulo);
                    last=tokens.length()-1;
                    if((tokens[last]==' ' || tokens[last]=='\t')){
                        while(tokens[last]==' ' || tokens[last]=='\t'){last--;}
                        tokens=tokens.substr(0,last+1);
                    }

                 //ERROS PARA LABEL
                    for(auto p : instructions){
                        if(tokens == p.first){diretiva=true;}    
                    }
                    if(diretiva){error=true;cout<<"linha "<<addr+1<<":: "<<"erro sintatico: instrucoes fora de ordem (diretiva) ->["<<tokens<<":]"<<endl;}  
                    if(tokens.find(' ')!=-1 && !diretiva){error=true;cout<<"linha "<<addr+1<<":: "<<"erro lexico: nome de rotulo invalido (space) ->["<<tokens<<"]"<<endl;}
                    if(tokens.find(':')!=-1 && !diretiva){error=true;cout<<"linha "<<addr+1<<":: "<<"erro lexico: nome de rotulo invalido (:) ->["<<tokens<<"]"<<endl;} 
                    for(auto p : tokens){
                        if(!isalnum(p) && p!='_' && p!=':'){inv_dgt=true;}
                    }
                    if(inv_dgt && !diretiva){error=true;cout<<"linha "<<addr+1<<":: "<<"erro lexico: nome de rotulo invalido (caracteres invalidos) ->["<<tokens<<"]"<<endl;}
                
                 //INSTRUCAO NA LABEL
                    instr=tokens_linha.substr(rotulo+1,tokens_linha.length());
                    if(instr=="\0" && !error){label_enter=true;}
                    if(!error){ehLabel=true;}
                }
                 
                 //INSTRUCAO SEM LABEL
                    if(rotulo==-1){ 
                        first=0;
                        instr=tokens_linha;
                        if(instr[first]!=' ' || instr[first]!='\t'){
                            while(instr[first]!=' ' && instr[first]!='\t'){first++;}
                            inst=instr.substr(0,first);
                        } 
                        for(auto p : instructions){
                            if(inst==p.first){diretiva=true;}       
                        }     
                    }   
                    
                    if(!label_enter){
                        first=0;
                        if((instr[first]==' ' || instr[first]=='\t')){
                            while(instr[first]==' ' || instr[first]=='\t'){first++;}
                            instr=instr.substr(first,instr.length()-1);
                        }

                        if(isdigit(instr[0])){error=true;cout<<"linha "<<addr+1<<":: "<<"erro lexico: primeiro caractere da instrucao invalido ->["<<instr<<"]"<<endl;}
                        else{
                            rotulo = instr.find(' ');
                            
                            
                            if(rotulo!=-1){     
                                inst=instr.substr(0,rotulo);
                                for(auto p : instructions){
                                    if(inst==p.first){diretiva=true;}    
                                }
                                
                            }else{
                                inst=instr;
                                for(auto p : instructions){
                                    if(inst==p.first){diretiva=true;}    
                                }
                            }
                            if(!error && !diretiva){
                                for(auto p : inst){if(isdigit(p)){error=true;}}
                                if(error){cout<<"linha "<<(addr+1)<<":: erro lexico: operacao inexistente ->["<<inst<<"]"<<endl;}
                                else{cout<<"linha "<<(addr+1)<<":: erro sintatico: operacao inexistente ->["<<inst<<"]"<<endl;}
                                error=true;
                            }

                        //OPERANDOS NA DIRETIVA
                            if(diretiva){                            
                                rotulo = instr.find(' ');
                                if(rotulo!=-1){
                                    operandos=instr.substr(rotulo,instr.length()-1);
                                }else{
                                    rotulo = instr.find(inst);
                                    operandos=instr.substr(rotulo+inst.length());
                                }
                                

                                first=0;
                                if((operandos[first]==' ' || operandos[first]=='\t')){
                                    while(operandos[first]==' ' || operandos[first]=='\t'){first++;}
                                    operandos=operandos.substr(first,operandos.length()-1);
                                }

                                if(inst=="CONST"){
                                    bool const_error=false;      
                                    num=0;
                                    int virg=operandos.find(',');
                                    if(virg!=-1 || operandos=="\0"){error=true;const_error=true;cout<<"linha "<<addr+1<<":: "<<"erro sintatico: numero indevido de operandos (op_CONST) ->["<<operandos<<"]"<<endl;}
                                    rotulo=operandos.find('-');
                                    if(rotulo!=operandos.find_last_of('-') && !const_error){error=true;cout<<"linha "<<addr+1<<":: "<<"erro lexico: operando invalido para instrucao (sub_CONST) ->["<<operandos<<"]"<<endl;}
                                    for(auto p : operandos){
                                        if(isdigit(p)){break;}
                                        num++;
                                    }
                                    for(auto p : operandos){
                                        if(!isdigit(p) && p!='-' && p!=' ' && p!='\t' && !const_error){const_error=true;error=true;cout<<"linha "<<addr+1<<":: "<<"erro sintatico: operando invalido para instrucao (alpha_CONST) ->["<<operandos<<"]"<<endl;break;}
                                    }
                                    if(rotulo>num && !const_error){error=true;cout<<"linha "<<addr+1<<":: "<<"erro lexico: operando invalido para instrucao (sub_CONST) ->["<<operandos<<"]"<<endl;}
                                    else if((rotulo==operandos.find_last_of('-')) && rotulo!=-1 && !const_error){
                                        num=(operandos.length());
                                        while(num--){
                                            if(isdigit(operandos[num])){break;}
                                        }

                                        operandos=operandos.substr(1,num);

                                        first=0;
                                        if((operandos[first]==' ' || operandos[first]=='\t')){
                                            while(operandos[first]==' ' || operandos[first]=='\t'){first++;}
                                            operandos=operandos.substr(first,operandos.length()-1);
                                        }
                                                
                                        for(auto p : operandos){
                                            if(!isdigit(p)){num=isdigit(p);error=true;cout<<"linha "<<addr+1<<":: "<<"erro sintatico: operando invalido para instrucao (!digit_CONST) ->["<<operandos<<"]"<<endl;break;}
                                        }
                                        if(num){
                                            if(rotulo==-1){
                                                num = stoi(operandos);
                                            }else{
                                                num = stoi(operandos);
                                                num=num*(-1);
                                            }
                                        }   
                                    }
                                    if(rotulo==-1 && !const_error){
                                        num = stoi(operandos);   
                                    }


                                }
                                else if(inst=="SPACE" || inst=="STOP"){
                                    if(operandos!="\0"){error=true;cout<<"linha "<<addr+1<<":: "<<"erro sintatico: esta operacao nao pode receber operandos (op_SPACE/STOP) ->["<<inst<<"]"<<endl;}
                                    
                                }
                                else if(inst=="COPY"){
                                    int virg=operandos.find(',');
                                    if(virg==-1 || virg==0 || virg==(operandos.length()-1) || operandos=="\0"){error=true;cout<<"linha "<<addr+1<<":: "<<"erro sintatico: numero indevido de operandos (op_COPY) ->["<<operandos<<"]"<<endl;}
                                    else if(operandos.find(' ')!=-1 || operandos.find('\t')!=-1){error=true;cout<<"linha "<<addr+1<<":: "<<"erro lexico: operando invalido (spc_COPY) ->["<<operandos<<"]"<<endl;}
                                    else{
                                        if(isdigit(operandos[0]) || isdigit(operandos[virg+1])){error=true;cout<<"linha "<<addr+1<<":: "<<"erro lexico: operando invalido (isdigit_COPY) ->["<<operandos<<"]"<<endl;}
                                        for(auto p: operandos){
                                            if(!isalnum(p) && p!=' ' && p!='\t' && p!=',' && p!='_'){error=true;cout<<"linha "<<addr+1<<":: "<<"erro lexico: operando invalido (!alnum_COPY) ->["<<operandos<<"]"<<endl;break;}
                                        }
                                    }
                                    if(!error){
                                        op1=operandos.substr(0,virg);
                                        op2=operandos.substr(virg+1,operandos.length()-1);
                                    }

                                }
                                else if(inst=="BEGIN"){}
                                else if(inst=="EXTERN"){
                                    tabUso_check.push_back(make_pair(tokens,0));
                                
                                }
                                else if(inst=="PUBLIC"){
                                    tabDef.push_back(make_pair(operandos,0));
                                }
                                else if(inst=="END"){}
                                else{
                                    
                                    int virg=operandos.find(',');
                                    if(virg!=-1 || operandos=="\0"){error=true;cout<<"linha "<<addr+1<<":: "<<"erro sintatico: numero indevido de operandos (op_"<<inst<<") ->["<<operandos<<"]"<<endl;}
                                    else if(operandos.find(' ')!=-1 || operandos.find('\t')!=-1){error=true;cout<<"linha "<<addr+1<<":: "<<"erro lexico: operando invalido (spc_"<<inst<<") ->["<<operandos<<"]"<<endl;}
                                    else{
                                        
                                        if(isdigit(operandos[0])){error=true;cout<<"linha "<<addr+1<<":: "<<"erro lexico: operando invalido (isdigit_"<<inst<<") ->["<<operandos<<"]"<<endl;}
                                        for(auto p: operandos){
                                            if(!isalnum(p) && p!=' ' && p!='\t' && p!='_'){error=true;cout<<"linha "<<addr+1<<":: "<<"erro lexico: operando invalido (!alnum_"<<inst<<") ->["<<operandos<<"]"<<endl;break;}
                                        }
                                    }
                                }                              
                            }
                        }       
                    } 
                    if(!error || label_enter){}  
                   
            }
        } 
        else{enter=true;} 

        if(ehLabel){
            label=tokens;
        }

     //VERIFICACAO DE SEMANTICA 
      //CRIANDO VETORES DE [] DE LABEL
        if(ehLabel){semantica_labels.push_back(label);label_linha.push_back(make_pair(label,addr+1));}
        

        if(!error && !isdigit(operandos[0])){
            if(operandos!="\0"){
                if(inst=="COPY"){semantica_operandos.push_back(op1);semantica_operandos.push_back(op2);operandos_linha.push_back(make_pair(op1,addr+1));operandos_linha.push_back(make_pair(op2,addr+1));}
                else{semantica_operandos.push_back(operandos);operandos_linha.push_back(make_pair(operandos,addr+1));}
            }  
        }
         
      //TIRA DUPLICATOS DE VETOR DE USO
        sort(semantica_operandos.begin(), semantica_operandos.end());

     //TABELA DE LINHAS 
        if(label_enter){
            passou=true;
            LINE.push_back(LINHA()); 
            LINE.at(ADR).label=(label);
        }
        else if(enter && passou && !diretiva){
        }
        else if(enter && !passou){
        }
        else if(!enter && !ehLabel && diretiva && passou){
            if(inst=="COPY"){
                LINE.at(ADR).instruction=(inst);
                LINE.at(ADR).op1=(op1);
                LINE.at(ADR).op2=(op2);
            }
            else if(inst=="CONST"){
                LINE.at(ADR).instruction=(inst);
                LINE.at(ADR).op1=(to_string(num));
                LINE.at(ADR).op2=("\0");
            }
            else{
                LINE.at(ADR).instruction=(inst);
                LINE.at(ADR).op1=(operandos);
                LINE.at(ADR).op2=("\0"); 
            }
            ADR++;
            passou=false;
        }
        else if(ehLabel && !label_enter && passou){
            if(inst=="COPY"){
                LINE.at(ADR).instruction=(inst);
                LINE.at(ADR).op1=(op1);
                LINE.at(ADR).op2=(op2);
                ADR++;
                LINE.push_back(LINHA()); 
                LINE.at(ADR).label=(label);
                LINE.at(ADR).instruction=(inst);
                LINE.at(ADR).op1=(op1);
                LINE.at(ADR).op2=(op2);
            }
            else if(inst=="CONST"){ 
                LINE.at(ADR).instruction=(inst);
                LINE.at(ADR).op1=(to_string(num));
                LINE.at(ADR).op2=("\0");
                ADR++;
                LINE.push_back(LINHA());
                LINE.at(ADR).label=(label);
                LINE.at(ADR).instruction=(inst);
                LINE.at(ADR).op1=(to_string(num));
                LINE.at(ADR).op2=("\0");
            }
            else{
                LINE.at(ADR).instruction=(inst);
                LINE.at(ADR).op1=(operandos);
                LINE.at(ADR).op2=("\0");
                ADR++;
                LINE.push_back(LINHA()); 
                LINE.at(ADR).label=(label);
                LINE.at(ADR).instruction=(inst);
                LINE.at(ADR).op1=(operandos);
                LINE.at(ADR).op2=("\0");
            }
            passou=false;
        }else{

            if(inst=="COPY" && ehLabel){ 
                LINE.push_back(LINHA()); 
                LINE.at(ADR).label=(label);
                LINE.at(ADR).instruction=(inst);
                LINE.at(ADR).op1=(op1);
                LINE.at(ADR).op2=(op2);
            }
            else if(inst=="COPY" && !ehLabel){
                LINE.push_back(LINHA()); 
                LINE.at(ADR).label=("\0");
                LINE.at(ADR).instruction=(inst);
                LINE.at(ADR).op1=(op1);
                LINE.at(ADR).op2=(op2);
            }
            else if(inst=="CONST" && ehLabel){
                LINE.push_back(LINHA()); 
                LINE.at(ADR).label=(label);
                LINE.at(ADR).instruction=(inst);
                LINE.at(ADR).op1=(to_string(num));
                LINE.at(ADR).op2=("\0");
            }
            else if(inst=="CONST" && !ehLabel){
                LINE.push_back(LINHA()); 
                LINE.at(ADR).label=("\0");
                LINE.at(ADR).instruction=(inst);
                LINE.at(ADR).op1=(to_string(num));
                LINE.at(ADR).op2=("\0");
            }
            else if(ehLabel){  
                LINE.push_back(LINHA()); 
                LINE.at(ADR).label=(label);
                LINE.at(ADR).instruction=(inst);
                LINE.at(ADR).op1=(operandos);
                LINE.at(ADR).op2=("\0");
            }
            else{
                LINE.push_back(LINHA()); 
                LINE.at(ADR).label=("\0");
                LINE.at(ADR).instruction=(inst);
                LINE.at(ADR).op1=(operandos);
                LINE.at(ADR).op2=("\0");
            }
            ADR++;
        }    

        tokens.clear(); 
        instr.clear();
        inst.clear();
        operandos.clear();  
        if(error){error_program=true;} 
    }

    sort(operandos_linha.begin(), operandos_linha.end());
    sort(label_linha.begin(), label_linha.end());
    sort(semantica_labels.begin(), semantica_labels.end());
    sort(semantica_operandos.begin(), semantica_operandos.end());    

    vector<string> sem_lbl;
    for(auto p : semantica_labels){sem_lbl.push_back(p);}

     //INICIA VERIFICACAO SEMANTICA
      //ERRO DE NAO DEFINIDO
      int contador_linhas=0;
      string uti;
        for(auto utilizadas : semantica_operandos){
            bool achoudefinicao=false;
            for(auto definidas : sem_lbl){
                if(definidas==utilizadas){achoudefinicao=true;}
                uti=utilizadas;
            }
            if(!achoudefinicao){error_program=true;cout<<"linha "<<operandos_linha.at(contador_linhas).second<<":: erro semantico: rotulo ["<<uti<<"] utilizado e nao definido"<<endl;}
            contador_linhas++;
        }
        
      //ERRO DE REDEFINICAO
        vector<string> redefinido;
        for(auto p : semantica_labels){
            redefinido.push_back(p);
        }
        contador_linhas=0;
        for(auto p : semantica_labels){
            auto rep1=find(semantica_labels.begin(),semantica_labels.end(),p);
            auto rep2=find(semantica_labels.rbegin(),semantica_labels.rend(),p);
            if(&(*rep1) != &(*rep2)){error_program=true;cout<<"linha "<<label_linha.at(contador_linhas).second<<":: erro semantico: tentativa de redefinir rotulo ["<<*rep1<<"]"<<endl;}
            contador_linhas++;
        }

             
 //SINTESE
   if(!error_program){
        //TABELA DE SIMBOLOS
        vector<pair<string,int>> Tsimbolos; // .first=simbolo .second=endereco

     //SECTION DATA
        vector<LINHA> DATA;
        counter=0;
        int flag=0;
        bool beg=false;
        for(auto linha : LINE){
            if(linha.instruction!="BEGIN" && module!=1){flag++;}
            else if(module == 1){flag++;}
            else{beg=true;}
            if(module!=1 && linha.instruction=="\0" && linha.label=="\0" && linha.op1=="\0" && linha.op2=="\0" && flag>1 && beg){break;}
            if(module==1 && linha.instruction=="\0" && linha.label=="\0" && linha.op1=="\0" && linha.op2=="\0" && flag>1){break;}
            if(linha.instruction=="\0" && linha.label=="\0" && linha.op1=="\0" && linha.op2=="\0" || linha.instruction=="BEGIN" || linha.instruction=="EXTERN" || linha.instruction=="PUBLIC" ){}
            else{
                DATA.push_back(LINHA());
                DATA.at(counter).label=linha.label;
                DATA.at(counter).instruction=linha.instruction;
                DATA.at(counter).op1=linha.op1;
                DATA.at(counter).op2=linha.op2;
                counter++;
            }
        }
        
        /*//IMPRESSÃO DA DATA
        cout<<"CODE BEGINS---------------------------------------------"<<endl; 
        int lna=0;
        for(auto linha : DATA){
            //cout<<"["<<linha.instruction<<"]"<<endl;
            if(linha.instruction=="\0"){cout<<lna<<" | _______________(SECTION)_______________"<<endl;}else{
            cout<<lna<<" | "<<linha.label<<" "<<linha.instruction<<" "<<linha.op1<<" "<<linha.op2<<endl;}
            lna++;
        }
        cout<<"END OF DATA"<<endl;*/
        
        //SECTION TEXT
        vector<LINHA> TEXT;
        counter=0;
        flag=0;
        for(auto linha : LINE){
            if(linha.instruction=="\0" && linha.label=="\0" && linha.op1=="\0" && linha.op2=="\0"){flag++;}
            if(flag==2){
            flag++;    
            }
            else if(flag==3 && linha.instruction!="END"){
                TEXT.push_back(LINHA());
                TEXT.at(counter).label=linha.label;
                TEXT.at(counter).instruction=linha.instruction;
                TEXT.at(counter).op1=linha.op1;
                TEXT.at(counter).op2=linha.op2;
                counter++;
            }
        }
        
        /*//IMPRESSÃO DO TEXT
        int lna=0;
        for(auto linha : TEXT){
            if(linha.instruction=="\0"){cout<<lna<<" | _______________(SECTION)_______________"<<endl;}else{
            cout<<lna<<" | "<<linha.label<<" "<<linha.instruction<<" "<<linha.op1<<" "<<linha.op2<<endl;}
            lna++;
        }
        cout<<"-----------------------------------------------CODE ENDS"<<endl;*/

        //CODIGO OBJETO
        struct OBJ {   
            int endereco;
            int opcode;
            int var;
            int var2;
        }; 
        vector<OBJ> OBJETO;
        OBJETO.push_back(OBJ());
        OBJETO.at(0).endereco=1;
        
        //PREENCHENDO ENDERECOS EM OBJ
        int edr=1,edraux;
        counter=1;
        for(auto linha : LINE){
            if(linha.instruction=="COPY"){
                OBJETO.push_back(OBJ());
                edraux=3;
                edr+=edraux;
                OBJETO.at(counter).endereco=edr;
            }
            else if(linha.instruction=="SPACE" || linha.instruction=="STOP" || linha.instruction=="CONST"){
                OBJETO.push_back(OBJ());
                edraux=1;
                edr+=edraux;
                OBJETO.at(counter).endereco=edr;    
            }
            else if(linha.instruction=="\0" || linha.instruction=="END" || linha.instruction=="BEGIN" || linha.instruction=="EXTERN" || linha.instruction=="PUBLIC"){}
            else{
                OBJETO.push_back(OBJ());
                edraux=2;
                edr+=edraux;
                OBJETO.at(counter).endereco=edr;
            }
            if(linha.instruction!="\0" && linha.instruction!="END" && linha.instruction!="BEGIN" && linha.instruction!="EXTERN" && linha.instruction!="PUBLIC"){counter++;}  
        }

        //ALTERANDO ENDERECOS PARA COMECAR EM 0
        int ln=0;
        for(auto obj : OBJETO){
            OBJETO.at(ln).endereco-=1;
            ln++;
        }

        //PREENCHENDO OP EM OBJ
        counter=0;
        int op;
        for(auto linha : LINE){
            for(auto p : instructions){
                if(linha.instruction==p.first && linha.instruction!="END" && linha.instruction!="BEGIN" && linha.instruction!="EXTERN" && linha.instruction!="PUBLIC"){
                    op=p.second;
                    OBJETO.at(counter).opcode=op;
                    counter++;    
                }       
            } 
        }
        
        //PREENCHENDO VAR EM OBJ
        bool declar_data=false,declar_txt=false;
        int var;
        int datasize=DATA.size();
        int txtsize=TEXT.size();
        counter=0;
        int counter2 = datasize;
        for(auto linha : TEXT){
            counter=0;
            if(linha.instruction!="\0"){
                //cout<<"-->"<<linha.instruction<<endl;
                for(auto data : DATA){
                    if(linha.op1==data.label){
                        //cout<<counter<<"-"<<linha.instruction<<"-"<<data.label<<endl;
                        var=OBJETO.at(counter).endereco;
                        //cout<<var<<endl;
                        declar_data=true;
                        break;      
                    }else{declar_data=false;}
                    counter++;   
                } 
                if(declar_data){
                    //cout<<counter<<"-"<<linha.instruction<<"-"<<linha.op1<<endl;
                    OBJETO.at(counter2).var=var;
                    counter2++;
                }else{
                    counter=0;
                    for(auto txt : LINE){
                        if(txt.instruction!="\0" && txt.instruction!="END" && txt.instruction!="BEGIN" && txt.instruction!="EXTERN" && txt.instruction!="PUBLIC"){
                            if(linha.op1==txt.label && counter>=datasize && txt.label!="\0"){
                                var=OBJETO.at(counter).endereco;
                                declar_txt=true;
                                break;
                            }else{declar_txt=false;}
                            if(txt.instruction!="END" && txt.instruction!="BEGIN" && txt.instruction!="EXTERN" && txt.instruction!="PUBLIC"){counter++;}
                        }
                    }
                }
                if(declar_txt){
                    counter=datasize;
                    for(auto txt : TEXT){
                        if(txt.op1==linha.op1){OBJETO.at(counter).var=var;counter2++;}
                        counter++;
                    }
                }
                else if(!declar_data && !declar_txt){}
                declar_data=false;
                declar_txt=false;
            }
        }
        
        //ADICIONANDO SEGUNDA VAR AO COPY
        ln=0;
        counter=datasize;
        for(auto txt : TEXT){
            ln=0;
            if(txt.instruction=="COPY"){
                //cout<<"entrei"<<endl;
                for(auto data : DATA){
                    //cout<<txt.op2<<" "<<data.label<<endl;
                    if(txt.op2==data.label){
                        //cout<<"------>"<<txt.op2<<endl;
                        OBJETO.at(counter).var2=ln;
                    }
                    ln++;
                }
            }
            counter++;
        }
        OBJETO.pop_back();

        /*//IMPRESSÃO DO CÓDIGO OBJETO DATA+TEXT (nessa ordem)
        cout<<endl<<"***************CODIGO OBJETO***************"<<endl;
        ln=0;
        for(auto linha : OBJETO){
            cout<<ln<<" | "<<linha.endereco<<" "<<linha.opcode<<" "<<linha.var<<" "<<linha.var2<<endl;
            ln++;
        }*/
        
        //INVERTENDO O CODIGO OBJETO
        vector<OBJ> OBJINV;
        int objetosize=OBJETO.size(); 
        ln=0;
        for(int i=0; i<objetosize; i++){
            OBJINV.push_back(OBJ());
        }

        //ADICIONANDO DATA SECTION
        counter=0;
        ln=0;
        for(auto obj : OBJETO){
            if(counter>=datasize){
                OBJINV.at(ln).endereco=obj.endereco;
                OBJINV.at(ln).opcode=obj.opcode;
                OBJINV.at(ln).var=obj.var;
                if(obj.var2!=0){OBJINV.at(ln).var2=obj.var2;}
                ln++;
            }
            counter++;
        }
        
        //ADICIONANDO TEXT SECTION
        counter=0;
        for(auto obj : OBJETO){
            if(counter<datasize){
                OBJINV.at(ln).endereco=obj.endereco;
                OBJINV.at(ln).opcode=obj.opcode;
                OBJINV.at(ln).var=obj.var;
                if(obj.var2!=0){OBJINV.at(ln).var2=obj.var2;}
                ln++;
            }else{break;}
            counter++;
        }
        
        //ARRUMANDO OS ENDERECOS
        ln=0;
        int endoftxt=0;
        for(auto obj : OBJINV){
            if(ln<txtsize){
                OBJINV.at(ln).endereco-=datasize;
                endoftxt=OBJINV.at(ln).endereco;
                ln++;
            }else{
                OBJINV.at(ln).endereco+=(txtsize)*2;
                ln++;
            }
        }
        //cout<<"aqui"<<datasize<<endl;
    
        //TRATAMENTO DE CASO PARA ENDERECOS DE SECTION DATA
        int dtsz = datasize,txtsz=txtsize;
        ln=0;
        counter=0;
        for(auto obj : OBJINV){
            if(obj.opcode==14  && txtsize>1){
                //cout<<OBJINV.at(ln+1).endereco<<"----------"<<obj.endereco<<endl;
                if(OBJINV.at(ln+1).endereco==obj.endereco+2){
                    ln++;
                    while(dtsz--){
                        OBJINV.at(ln).endereco-=1;
                        ln++;
                    }
                    break;
                }
            }
            ln++;        
        }
        /*cout<<endl<<"***************CODIGO OBJETO INVERTIDO***************"<<endl;
        ln=0;
        for(auto linha:OBJINV){cout<<ln<<" | "<<linha.endereco<<" _ "<<linha.opcode<<" _ "<<linha.var<<" _ "<<linha.var2<<endl;ln++;}*/ 

        //ACHA O ENDEREÇO DE INICIO DE SECTION DATA 
        int endofvar=0,begofdata=0,countdata=0;
        for(auto aux : OBJINV){
            //cout<<aux.endereco<<endl;
            if(aux.endereco==endoftxt){
                if(aux.endereco!=0){begofdata=OBJINV.at(countdata+1).endereco;}
            }
            countdata++;
        }
        //cout<<"beg->"<<begofdata<<endl;
        

        //ARRUMANDO AS VAR
        ln=0;
        int cntdt=0;
        //cout<<datasize<<"dt txt"<<txtsize<<endl;
        for(auto obj : OBJINV){
            //cout<<"obj "<<obj.var<<endl;
            if(ln<txtsize){
                if(obj.var<=datasize-1){
                    //cout<<obj.var<<" ----- "<<ln<<" "<<OBJINV.at(3).var<<endl;
                    OBJINV.at(ln).var+=begofdata;
                    //OBJINV.at(ln).var+=(txtsize)*2;
                    ln++;
                }else{
                    for(auto dt : OBJETO){if(dt.endereco==obj.var){break;};cntdt++;}
                    //cout<<obj.var<<"var cnt"<<cntdt<<endl;
                    cntdt-=datasize;
                    //cout<<obj.var<<"var cnt"<<cntdt<<" "<<OBJINV.at(ln).var<<" "<<OBJINV.at(cntdt).endereco<<endl;
                    OBJINV.at(ln).var=OBJINV.at(cntdt).endereco;
                    //cout<<OBJINV.at(ln).var<<endl;
                    cntdt=0;
                    ln++;
                }
            }
        }
        
        //ARRUMANDO AS VAR 2
        ln=0;
        cntdt=0;
        for(auto obj : OBJINV){
            if(ln<txtsize){
                if(obj.var2<datasize){
                    if(obj.var2!=0){OBJINV.at(ln).var2+=begofdata;}
                    ln++;
                }else{
                    if(obj.var2!=0){
                        for(auto dt : OBJETO){if(dt.endereco==obj.var2){break;};cntdt++;}
                        //cout<<obj.var<<"var cnt"<<cntdt<<endl;
                        cntdt-=datasize;
                        //cout<<obj.var<<"var cnt"<<cntdt<<" "<<OBJINV.at(ln).var<<" "<<OBJINV.at(cntdt).endereco<<endl;
                        OBJINV.at(ln).var2=OBJINV.at(cntdt).endereco;
                        //cout<<OBJINV.at(ln).var<<endl;
                        cntdt=0;
                    }
                    ln++;
                }
            }
        }

        //ZERANDO VARIAVEL DE INSTRUCAO STOP
        ln=0;
        for(auto obj : OBJINV){
            if(obj.opcode==14){OBJINV.at(ln).var=0;OBJINV.at(ln).var2=0;}
            ln++;
        }

        //ADICIONANDO VALORES DE CONST
        ln=txtsize;
        for(auto data : DATA){
            if(data.instruction=="CONST"){OBJINV.at(ln).opcode=stoi(data.op1);}
            ln++;
        } 

        
        //TABELA DE DEFINIÇÕES - PREENCHIMENTO DE VALORES 
        if(module != 1){
            counter2 = 0;
            for(auto tdef : tabDef){
                bool defTxt = false, defDt = false;
                counter = 0;
                for(auto linha : TEXT){
                    if(tdef.first==linha.label){
                        //cout<<"---"<<linha.label<<" "<<counter<<" "<<counter2<<endl;
                        tabDef.at(counter2).second = OBJINV.at(counter).endereco;
                        counter2++;
                        defTxt=true;
                    }
                    //cout<<counter<<" -counter- "<<counter2<<endl;
                    counter++;
                }  
                if(!defTxt){
                    counter = TEXT.size();  
                    for(auto linha : DATA){
                        if(tdef.first==linha.label){
                            //cout<<"---"<<linha.label<<" "<<counter<<" "<<counter2<<endl;
                            tabDef.at(counter2).second = OBJINV.at(counter).endereco;
                            counter2++;
                            defDt=true;
                        }
                        //cout<<counter<<" +counter+ "<<counter2<<endl;
                        counter++;
                    }
                    if(!defTxt && !defDt){tabDef.at(counter2).second = 0; counter2++;}
                }    
                
            }
        } 


        //TABELA DE USO - PREENCHIMENTO DE ENDEREÇOS
        if(module != 1){
            counter2 = 0;
            for(auto tuso : tabUso_check){
                counter = 0;
                for(auto linha : TEXT){
                    //cout<<counter<<" cntr "<<counter2<<endl;
                    if(tuso.first==linha.op1){
                        //cout<<tuso.first<<endl;
                        tabUso.push_back(make_pair(tuso.first , OBJINV.at(counter).endereco+1));
                        OBJINV.at(counter).var = 0;
                        counter2++;
                    }
                    counter++;
                } 
                counter=0; 
                for(auto linha : TEXT){
                    //cout<<counter<<" cntr+ "<<counter2<<endl;
                    if(tuso.first==linha.op2){
                        //cout<<tuso.first<<endl;
                        tabUso.push_back(make_pair(tuso.first , OBJINV.at(counter).endereco+2));
                        OBJINV.at(counter).var2 = 0;
                        counter2++;
                    }
                    counter++;
                }
            }
        }




        /*//IMPRESSÃO DO CÓDIGO OBJETO, TABELA DE DEFINIÇÕES E TABELA DE USO
        cout<<endl<<"***************CODIGO OBJETO INVERTIDO***************"<<endl;
        ln=0;
        for(auto linha:OBJINV){cout<<ln<<" | "<<linha.endereco<<" _ "<<linha.opcode<<" _ "<<linha.var<<" _ "<<linha.var2<<endl;ln++;} 
        cout<<"++++++++++++TABELA DE DEFINICAO++++++++++++"<<endl;
        for(auto a : tabDef){
            cout<<"["<<a.first<<"] ["<<a.second<<"]"<<endl;
        }
        cout<<"+++++++++++++++TABELA DE USO+++++++++++++++"<<endl;
        for(auto a : tabUso){
            cout<<"["<<a.first<<"] ["<<a.second<<"]"<<endl;
        }
        cout<<"+++++++++++++++++++++++++++++++++++++++++++"<<endl;*/


        //INSERINDO CODIGO OBJETO EM ARQUIVO .OBJ
        string newarq=arq;
        auto a = newarq.find(".asm");
        if(a!=-1){newarq.replace(a,4,".mon");}
        //cout<<endl<<"newarq->"<<newarq<<endl;
        
        //Nome do programa
        arq.erase(arq.end()-4,arq.end());

        //Contagem do tamanho do programa
        int arqTam = (2*txtsize)+datasize;
        counter = 1;
        for(auto linha:OBJINV){if(linha.opcode==14 && counter<=txtsize){arqTam-=1;}; if(linha.opcode==9 && counter<=txtsize){arqTam+=1;}; counter++;}

        //Mapa de bits
        string arqMapBit;
        counter = 1;
        for(auto linha:OBJINV){
            if(counter<=txtsize){
                if(linha.opcode != 14){arqMapBit.push_back('0');}
                if(linha.var != 0){arqMapBit.push_back('1');}
                else{arqMapBit.push_back('0');}
                if(linha.var2 != 0){arqMapBit.push_back('1');}
                else if(linha.var2 == 0 && linha.opcode == 9){arqMapBit.push_back('0');}
            }else{arqMapBit.push_back('0');} 
            counter++;
        }
        //cout<<arqMapBit<<endl;

        ofstream MyFile(newarq);

        // Write to the file
        counter = 1;
        bool tu=false,td=false;
        for(auto linha:OBJINV){
            if(counter == 1){
                MyFile<<"H "<<arq<<endl;
                MyFile<<"H "<<arqTam<<endl;
                MyFile<<"R "<<arqMapBit<<endl;
            }
            if(module > 1 && !tu){
                for(auto tu:tabUso){MyFile<<"U "<<tu.first<<" "<<tu.second<<endl;}
                tu=true;
            }
            if(module > 1 && !td){
                for(auto td:tabDef){MyFile<<"D "<<td.first<<" "<<td.second<<endl;}
                td=true;
            }

            if(module == 1 && counter <= txtsize){
                if(linha.opcode == 9){
                    MyFile<<"T "<<linha.opcode<<" "<<linha.var<<" "<<linha.var2<<endl;
                    counter++;
                }
                else if(linha.opcode == 14){MyFile<<"T "<<linha.opcode<<endl; counter++;}
                else{MyFile<<"T "<<linha.opcode<<" "<<linha.var<<endl; counter++;}
            }
            else if(module == 1 && counter > txtsize){MyFile<<"T "<<linha.opcode<<endl; counter++;}
            
            
            if(module > 1 && counter <= txtsize){
                if(linha.opcode == 9){
                    MyFile<<"T "<<linha.opcode<<" "<<linha.var<<" "<<linha.var2<<endl;
                    counter++;
                }
                else if(linha.opcode == 14){MyFile<<"T "<<linha.opcode<<endl; counter++;}
                else{MyFile<<"T "<<linha.opcode<<" "<<linha.var<<endl; counter++;}
            }
            else if(module > 1 && counter > txtsize){MyFile<<"T "<<linha.opcode<<endl; counter++;}
        } 
        
        // Close the file
        MyFile.close();   
   }
return true;
}

int main(int argc, char **argv){

    //VARIÁVEIS DA MAIN
    vector<string> cmdArguments(argv+1, argv + argc);
    string arq=argv[1];
    vector<string> linhas,linhas2,linhas3;

    //VERIFICANDO A QUANTIDADE DE ARQUIVOS RECEBIDOS
    bool erro=false;
    if(cmdArguments.size() == 1){
        //cout<<"um arquivo"<<endl; 
        getFileContent(arq,linhas);
        
        //procura por erro de BEGIN e END
        int Beg=0, End=0;
        for(auto busca : linhas){
            Beg = busca.find("BEGIN");
            End = busca.find("END");
            if(Beg != -1){cout<<"ERRO: arquivo nao pode conter \"BEGIN\""<<endl; erro=true;}
            if(End != -1){cout<<"ERRO: arquivo nao pode conter \"END\""<<endl; erro=true;}
            Beg = busca.find("begin");
            End = busca.find("end");
            if(Beg != -1){cout<<"ERRO: arquivo nao pode conter \"begin\""<<endl; erro=true;}
            if(End != -1){cout<<"ERRO: arquivo nao pode conter \"end\""<<endl; erro=true;}
        }

        //EXECUTA MONTAGEM
        if(!erro){exe(arq, linhas, 1);}
    }
    else if(cmdArguments.size() == 2){ 
        //cout<<"dois arquivos"<<endl; 
        getFileContent(arq,linhas); 
        getFileContent(argv[2],linhas2);
        
        //procura por erro de BEGIN e END arq1
        int Beg=0, End=0;
        bool beg=false, end=false;
        for(auto busca : linhas){
            Beg = busca.find("BEGIN");
            End = busca.find("END");
            if(Beg != -1){beg=true;}
            if(End != -1){end=true;}
            Beg = busca.find("begin");
            End = busca.find("end");
            if(Beg != -1){beg=true;}
            if(End != -1){end=true;}
        }
        if(!beg){cout<<"ERRO: arquivo 1 nao contem \"BEGIN\""<<endl;}
        if(!end){cout<<"ERRO: arquivo 1 nao contem \"END\""<<endl;}
        if(!beg || !end){erro=true;}

        //procura por erro de BEGIN e END arq2
        Beg=0, End=0;
        beg=false, end=false;
        for(auto busca : linhas2){
            Beg = busca.find("BEGIN");
            End = busca.find("END");
            if(Beg != -1){beg=true;}
            if(End != -1){end=true;}
            Beg = busca.find("begin");
            End = busca.find("end");
            if(Beg != -1){beg=true;}
            if(End != -1){end=true;}
        }
        if(!beg){cout<<"ERRO: arquivo 2 nao contem \"BEGIN\""<<endl;}
        if(!end){cout<<"ERRO: arquivo 2 nao contem \"END\""<<endl;}
        if(!beg || !end){erro=true;}

        //EXECUTA MONTAGEM
        if(!erro){
            exe(arq, linhas, 2);
            exe(argv[2], linhas2, 2);
        }
    }
    else if(cmdArguments.size() == 3){
        //cout<<"tres arquivos"<<endl; 
        getFileContent(arq,linhas); 
        getFileContent(argv[2],linhas2);
        getFileContent(argv[3],linhas3);
       
        //procura por erro de BEGIN e END arq1
        int Beg=0, End=0;
        bool beg=false, end=false;
        for(auto busca : linhas){
            Beg = busca.find("BEGIN");
            End = busca.find("END");
            if(Beg != -1){beg=true;}
            if(End != -1){end=true;}
            Beg = busca.find("begin");
            End = busca.find("end");
            if(Beg != -1){beg=true;}
            if(End != -1){end=true;}
        }
        if(!beg){cout<<"ERRO: arquivo 1 nao contem \"BEGIN\""<<endl;}
        if(!end){cout<<"ERRO: arquivo 1 nao contem \"END\""<<endl;}
        if(!beg || !end){erro=true;}

        //procura por erro de BEGIN e END arq2
        Beg=0, End=0;
        beg=false, end=false;
        for(auto busca : linhas2){
            Beg = busca.find("BEGIN");
            End = busca.find("END");
            if(Beg != -1){beg=true;}
            if(End != -1){end=true;}
            Beg = busca.find("begin");
            End = busca.find("end");
            if(Beg != -1){beg=true;}
            if(End != -1){end=true;}
        }
        if(!beg){cout<<"ERRO: arquivo 2 nao contem \"BEGIN\""<<endl;}
        if(!end){cout<<"ERRO: arquivo 2 nao contem \"END\""<<endl;}
        if(!beg || !end){erro=true;}

        //procura por erro de BEGIN e END arq3
        Beg=0, End=0;
        beg=false, end=false;
        for(auto busca : linhas3){
            Beg = busca.find("BEGIN");
            End = busca.find("END");
            if(Beg != -1){beg=true;}
            if(End != -1){end=true;}
            Beg = busca.find("begin");
            End = busca.find("end");
            if(Beg != -1){beg=true;}
            if(End != -1){end=true;}
        }
        if(!beg){cout<<"ERRO: arquivo 3 nao contem \"BEGIN\""<<endl;}
        if(!end){cout<<"ERRO: arquivo 3 nao contem \"END\""<<endl;}
        if(!beg || !end){erro=true;}

        //EXECUTA MONTAGEM
        if(!erro){
            exe(arq, linhas, 3);
            exe(argv[2], linhas2, 3);
            exe(argv[3], linhas3, 3);
        }
    }
    else{cout<<"NUMERO DE ARQUIVOS INDEVIDO"<<endl;}
    
    
    return 0;
}