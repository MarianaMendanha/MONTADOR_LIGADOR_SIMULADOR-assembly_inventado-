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


#define ADD(ACC, MEM_OP) (ACC + MEM_OP)
#define SUB(ACC, MEM_OP) (ACC - MEM_OP)
#define MUL(ACC, MEM_OP) (ACC * MEM_OP)
#define DIV(ACC, MEM_OP) (ACC / MEM_OP)
#define JMP(PC, OP) (PC = OP)
#define JUMPN(ACC, PC, OP) if(ACC<0){(PC = OP);}
#define JUMPP(ACC, PC, OP) if(ACC>0){(PC = OP);}
#define JUMPZ(ACC, PC, OP) if(ACC==0){(PC = OP);}
#define COPY(MEM_OP0, MEM_OP1) (MEM_OP1 = MEM_OP0)
#define LOAD(ACC, MEM_OP) (ACC = MEM_OP)
#define STORE(ACC, MEM_OP) (MEM_OP = ACC)
#define INPUT(MEM_OP) cin>>MEM_OP;
#define OUTPUT(MEM_OP) cout<<"OUTPUT: "<<MEM_OP<<endl;
#define STOP return 0;

#define PC(PC) cout<<"PC <- "<<PC<<endl;
#define ACC(ACC) cout<<"ACC <- "<<ACC<<endl;

using namespace std;

bool getFileContent(string arq, vector<int> &objeto){
    ifstream fin(arq.c_str());

    if(!fin){
        cerr << "Cannot open the File : "<<arq<<endl;
        return false;
    }

    int i;
    while(fin >> i){
        objeto.push_back(i);
    }
    

    fin.close();
return true;
}


int main(int argc, char **argv){

    struct OBJ{   
        int endereco;
        int opcode;
        int var;
        int var2;
    }; 
    vector<OBJ> instructions;
    vector<int> objeto;
    string arq=argv[1];
    bool result = getFileContent(arq,objeto);

    //NOME DO ARQUIVO
    //cout << arq <<endl;
    
    //CONTEUDO DO ARQUIVO
    //if(result){
    //    for(auto t : objeto){
    //        cout << t <<endl;
    //    }
    //}

    //INSERINDO ENDERECOS E SEPARANDO COMANDOS
    vector<int> addr;
    int counter=0,objsz=objeto.size(),i=0;
    for(i=0; i<objsz; i++){ 
        //cout<<i<<"*"<<objeto.at(i)<<"*"<<endl;
        if(objeto.at(i)==9){
            addr.push_back(i);
            instructions.push_back(OBJ());
            instructions.at(counter).endereco=i;
            instructions.at(counter).opcode=objeto.at(i);
            instructions.at(counter).var=objeto.at(i+1);
            instructions.at(counter).var2=objeto.at(i+2);
            //cout<<instructions.at(counter).endereco<<" "<<instructions.at(counter).opcode<<" "<<instructions.at(counter).var<<" "<<instructions.at(counter).var2<<endl;
            i+=2;
            counter++;
        }
        else if(objeto.at(i)==14){
            int resto = objsz-i;
            //cout<<"resto->"<<resto<<endl;
            while(resto--){
                addr.push_back(i);
                instructions.push_back(OBJ());
                instructions.at(counter).endereco=i;
                instructions.at(counter).opcode=objeto.at(i);
                //cout<<counter<<"->"<<instructions.at(counter).endereco<<" "<<instructions.at(counter).opcode<<endl;
                i++;
                counter++;
            }
        }
        else{
            addr.push_back(i);
            instructions.push_back(OBJ());
            instructions.at(counter).endereco=i;
            instructions.at(counter).opcode=objeto.at(i);
            instructions.at(counter).var=objeto.at(i+1);
            //cout<<instructions.at(counter).endereco<<" "<<instructions.at(counter).opcode<<" "<<instructions.at(counter).var<<endl;
            i++;
            counter++;
        }
    
    }

    vector<OBJ> ref;
    counter=0;
    for(auto linha : instructions){
        ref.push_back(OBJ());
        ref.at(counter).endereco=linha.endereco;
        ref.at(counter).opcode=linha.opcode;
        ref.at(counter).var=linha.var;
        ref.at(counter).var2=linha.var2;
        counter++;
    }

    //CODIGO OBJETO ENDERECADO
    /*cout<<endl<<"***************CODIGO OBJETO***************"<<endl;
    int ln=0;
    for(auto linha : ref){
        cout<<ln<<" | "<<linha.endereco<<") "<<linha.opcode<<" : "<<linha.var<<" _ "<<linha.var2<<endl;
        ln++;
    }
    cout<<"*******************************************"<<endl<<endl;*/

    //INSERINDO RESPOSTA AO CODIGO OBJETO EM ARQUIVO .TXT
    string newarq=arq;
    auto a = newarq.find(".obj");              
    if(a!=-1){newarq.replace(a,4,".out");}      //TROCA .OBJ POR .TXT 
    ofstream MyFile(newarq);                    //CRIA NOVO ARQUIVO
    

    //IMPLEMENTACAO DAS INSTRUCOES
    vector<int>::iterator it,it1;
    bool jump;
    int inssz=instructions.size();
    int acc=0,pc=0,op0,op1,aux=0,opaux;
    for(int i=0; i<instructions.size(); i++){
        jump=false;
        if(instructions.at(i).opcode==1){                //ADD
            op0=instructions.at(i).var;
            it=find(addr.begin(),addr.end(),op0);
            auto index = distance( addr.begin(), it);
            op0=instructions.at(index).opcode;
            acc=ADD(acc,op0);
            aux++; 
            pc=addr.at(aux);
            PC(pc);
            ACC(acc);
            cout<<endl;
        }
        else if(instructions.at(i).opcode==2){           //SUB
            op0=instructions.at(i).var;
            it=find(addr.begin(),addr.end(),op0);
            auto index = distance( addr.begin(), it);
            op0=instructions.at(index).opcode;
            acc=SUB(acc,op0);
            aux++; 
            pc=addr.at(aux);
            PC(pc);
            ACC(acc);
            cout<<endl;
        }
        else if(instructions.at(i).opcode==3){           //MUL
            op0=instructions.at(i).var;
            it=find(addr.begin(),addr.end(),op0);
            auto index = distance( addr.begin(), it);
            op0=instructions.at(index).opcode;
            acc=MUL(acc,op0);
            aux++; 
            pc=addr.at(aux);
            PC(pc);
            ACC(acc);
            cout<<endl;
        }
        else if(instructions.at(i).opcode==4){           //DIV
            op0=instructions.at(i).var;
            it=find(addr.begin(),addr.end(),op0);
            auto index = distance( addr.begin(), it);
            op0=instructions.at(index).opcode;
            acc=DIV(acc,op0);
            aux++; 
            pc=addr.at(aux);
            PC(pc);
            ACC(acc);
            cout<<endl;
        }
        else if(instructions.at(i).opcode==5){           //JMP
            op0=instructions.at(i).var;
            JMP(pc,op0);
            it=find(addr.begin(),addr.end(),pc);
            auto index = distance( addr.begin(), it);
            i=index-1;
            PC(pc);
            ACC(acc); 
            cout<<endl;
            pc=addr.at(index);
            aux=index;
            jump=true;
        }
        else if(instructions.at(i).opcode==6){           //JUMPN
            op0=instructions.at(i).var;
            JUMPN(acc,pc,op0);
            if(acc<0){jump=true;it=find(addr.begin(),addr.end(),pc);auto index = distance( addr.begin(), it);i=index-1;pc=addr.at(index);aux=index;}
            else{aux++;pc=addr.at(aux);}
            PC(pc);
            ACC(acc);
            cout<<endl;
        }
        else if(instructions.at(i).opcode==7){           //JUMPP
            op0=instructions.at(i).var;
            JUMPP(acc,pc,op0);
            if(acc>0){jump=true;it=find(addr.begin(),addr.end(),pc);auto index = distance( addr.begin(), it);i=index-1;pc=addr.at(index);aux=index;}
            else{aux++;pc=addr.at(aux);} 
            PC(pc);
            ACC(acc);
            cout<<endl;
        }
        else if(instructions.at(i).opcode==8){           //JUMPZ
            op0=instructions.at(i).var;
            JUMPZ(acc,pc,op0);
            if(acc==0){jump=true;it=find(addr.begin(),addr.end(),pc);auto index = distance( addr.begin(), it);i=index-1;pc=addr.at(index);aux=index;}
            else{aux++;pc=addr.at(aux);}
            PC(pc);
            ACC(acc);
            cout<<endl;
        }
        else if(instructions.at(i).opcode==9){           //COPY
            op0=instructions.at(i).var;
            it=find(addr.begin(),addr.end(),op0); 
            op1=instructions.at(i).var2;
            it1=find(addr.begin(),addr.end(),op1);
            auto index = distance( addr.begin(), it);
            auto index1 = distance( addr.begin(), it1);
            op0=instructions.at(index).opcode;
            COPY(op0,instructions.at(index1).opcode);
            aux++; 
            pc=addr.at(aux);
            PC(pc);
            ACC(acc);
            cout<<endl;
        }
        else if(instructions.at(i).opcode==10){          //LOAD
            op0=instructions.at(i).var;
            it=find(addr.begin(),addr.end(),op0);
            auto index = distance( addr.begin(), it);
            op0=instructions.at(index).opcode;
            LOAD(acc,op0);
            aux++; 
            pc=addr.at(aux);
            PC(pc);
            ACC(acc);
            cout<<endl;
        }
        else if(instructions.at(i).opcode==11){          //STORE
            op0=instructions.at(i).var;
            it=find(addr.begin(),addr.end(),op0);
            auto index = distance( addr.begin(), it);
            STORE(acc,instructions.at(index).opcode);
            aux++; 
            pc=addr.at(aux);
            PC(pc);
            ACC(acc);
            cout<<endl;
        }
        else if(instructions.at(i).opcode==12){          //INPUT
            op0=instructions.at(i).var;
            INPUT(opaux);
            it=find(addr.begin(),addr.end(),op0);
            auto index = distance( addr.begin(), it);         
            instructions.at(index).opcode=opaux;
            aux++; 
            pc=addr.at(aux);
            PC(pc);
            ACC(acc);
            cout<<endl;
        }
        else if(instructions.at(i).opcode==13){          //OUTPUT
            op0=instructions.at(i).var;
            it=find(addr.begin(),addr.end(),op0);
            auto index = distance( addr.begin(), it);
            op0=instructions.at(index).opcode;
            aux++; 
            pc=addr.at(aux);
            PC(pc);
            ACC(acc);
            OUTPUT(op0);
            cout<<endl;
            MyFile<<op0<<endl;
        }
        else if(instructions.at(i).opcode==14){aux++;pc=addr.at(aux);PC(pc);ACC(acc);STOP;}
    }
        
    MyFile.close();


return 0;
}