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

//CODIGO DE ARQUIVOS EM LINHAS
vector<string> linhas,linhas2,linhas3;
vector<int> codLig;

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

void exe(string arq, int module){
    vector<pair<string,int>> tabGDef,tabUso1, tabUso2, tabUso3;
    vector<int> fatCorr;
    vector<string> bitMap;
    
    //FATORES DE CORREÇÃO
    string Hers = linhas.at(1);
    Hers.erase(Hers.begin(),Hers.begin()+2);
    fatCorr.push_back(stoi(Hers));
    if(module==3){
        Hers = linhas2.at(1);
        Hers.erase(Hers.begin(),Hers.begin()+2);
        fatCorr.push_back(stoi(Hers));
    }
    
    //MAPA DE BITS,TABELAS DE USO, TABELA DE DEFINIÇÃO E COÓDIGO OBJ ARQ1
    for(auto linha:linhas){
        if(linha.at(0)=='R'){
            string Uers=linha;
            Uers.erase(Uers.begin(),Uers.begin()+2);
            bitMap.push_back(Uers);
        }
        if(linha.at(0)=='U'){
            string Uers=linha;
            Uers.erase(Uers.begin(),Uers.begin()+2);

            int pos = Uers.find(" ");
            tabUso1.push_back(make_pair(Uers.substr(0,pos), stoi(Uers.substr(pos+1))));
        }
        if(linha.at(0)=='D'){
            string Ders=linha;
            Ders.erase(Ders.begin(),Ders.begin()+2);
        
            int pos = Ders.find(" ");
            tabGDef.push_back(make_pair(Ders.substr(0,pos), stoi(Ders.substr(pos+1))));
        }
        int counter=0;
        if(linha.at(0)=='T'){
            string Ters=linha;
            Ters.erase(Ters.begin(),Ters.begin()+2);
            
            int pos = Ters.find(" ");
            int posf = Ters.rfind(" ");
            //cout<<Ters<<" +++ "<<stoi(Ters.substr(0,pos))<<endl;
            codLig.push_back(stoi(Ters.substr(0,pos)));
            if(pos != -1 && pos == posf){codLig.push_back(stoi(Ters.substr(pos+1))); /*cout<<Ters<<" "<<stoi(Ters.substr(pos+1))<<endl;*/}
            if(pos != posf){
                //cout<<Ters.at(posf)<<"|"<<stoi(Ters.substr(pos+1,Ters.size()-posf))<<"|"<<endl;
                //cout<<"---"<<Ters<<endl;
                codLig.push_back(stoi(Ters.substr(pos+1,Ters.size()-posf)));
                codLig.push_back(stoi(Ters.substr(posf+1)));
            }
            counter++;    
            
        }
    } 
   

    //MAPA DE BITS,TABELAS DE USO, TABELA DE DEFINIÇÃO E COÓDIGO OBJ ARQ2
    for(auto linha:linhas2){
        if(linha.at(0)=='R'){
            string Rers=linha;
            Rers.erase(Rers.begin(),Rers.begin()+2);
            bitMap.push_back(Rers);
        }
        if(linha.at(0)=='U'){
            string Uers=linha;
            Uers.erase(Uers.begin(),Uers.begin()+2);
            int pos = Uers.find(" ");
            tabUso2.push_back(make_pair(Uers.substr(0,pos), stoi(Uers.substr(pos+1))));
        }
        if(linha.at(0)=='D'){
            string Ders=linha;
            Ders.erase(Ders.begin(),Ders.begin()+2);
            
            int pos = Ders.find(" ");
            tabGDef.push_back(make_pair(Ders.substr(0,pos), stoi(Ders.substr(pos+1)) + fatCorr.at(0)));
        }
        if(linha.at(0)=='T'){
            string Ters=linha;
            Ters.erase(Ters.begin(),Ters.begin()+2);
            
            int pos = Ters.find(" ");
            int posf = Ters.rfind(" ");
            codLig.push_back(stoi(Ters.substr(0,pos)));
            if(pos != -1 && pos == posf){codLig.push_back(stoi(Ters.substr(pos+1)));}
            if(pos != posf){
                codLig.push_back(stoi(Ters.substr(pos+1,Ters.size()-posf)));
                codLig.push_back(stoi(Ters.substr(posf+1)));
            }
        }
    }
   
    //MAPA DE BITS,TABELAS DE USO, TABELA DE DEFINIÇÃO E COÓDIGO OBJ ARQ3
    if(module==3){
        //TABELAS DE USO E TABELA DE DEFINIÇÃO ARQ3
        for(auto linha:linhas3){
            if(linha.at(0)=='R'){
                string Rers=linha;
                Rers.erase(Rers.begin(),Rers.begin()+2);
                bitMap.push_back(Rers);
            }
            if(linha.at(0)=='U'){
                string Uers=linha;
                Uers.erase(Uers.begin(),Uers.begin()+2);
                int pos = Uers.find(" ");
                tabUso3.push_back(make_pair(Uers.substr(0,pos), stoi(Uers.substr(pos+1))));
            }
            if(linha.at(0)=='D'){
                string Ders=linha;
                Ders.erase(Ders.begin(),Ders.begin()+2);
                
                int pos = Ders.find(" ");
                tabGDef.push_back(make_pair(Ders.substr(0,pos), stoi(Ders.substr(pos+1))+ fatCorr.at(0) + fatCorr.at(1)));
            }
            if(linha.at(0)=='T'){
                string Ters=linha;
                Ters.erase(Ters.begin(),Ters.begin()+2);
                
                int pos = Ters.find(" ");
                int posf = Ters.rfind(" ");
                codLig.push_back(stoi(Ters.substr(0,pos)));
                if(pos != -1 && pos == posf){codLig.push_back(stoi(Ters.substr(pos+1)));}
                if(pos != posf){
                    codLig.push_back(stoi(Ters.substr(pos+1,Ters.size()-posf)));
                    codLig.push_back(stoi(Ters.substr(posf+1)));
                }
            }
        }
    }

    /*cout<<"CLIG-0--------------------------------"<<endl; 
    int counter1=1; 
    for(auto a:codLig){cout<<"|"<<a<<"|"; if(counter1==4){cout<<endl;counter1=0;}; counter1++;}
    cout<<endl<<"--------------------------------------"<<endl;*/

    
    /*//IMPRESSÃO DAS TABELAS
    cout<<"USO1----------------------------------"<<endl;  
    for(auto a:tabUso1){cout<<"|"<<a.first<<"|"<<a.second<<"|"<<endl;}
    cout<<"USO2----------------------------------"<<endl;  
    for(auto a:tabUso2){cout<<"|"<<a.first<<"|"<<a.second<<"|"<<endl;}
    cout<<"USO3----------------------------------"<<endl;  
    for(auto a:tabUso3){cout<<"|"<<a.first<<"|"<<a.second<<"|"<<endl;}
    cout<<"DEFG----------------------------------"<<endl;  
    for(auto a:tabGDef){cout<<"|"<<a.first<<"|"<<a.second<<"|"<<endl;}
    cout<<"CLIG----------------------------------"<<endl;  
    int counter=1; 
    for(auto a:codLig){cout<<"|"<<a<<"|"; if(counter==4){cout<<endl;counter=0;}; counter++;}
    cout<<endl;*/

    //RESOLUÇÃO DAS REFERÊNCIAS
    int counter=0;
    for(auto tu:tabUso1){
        int vlr=0;
        for(auto tgd:tabGDef){if(tgd.first==tu.first){vlr=tgd.second; break;}}
        codLig.at(tu.second) = vlr;
    }
    for(auto tu:tabUso2){
        int vlr=0;
        for(auto tgd:tabGDef){if(tgd.first==tu.first){vlr=tgd.second; break;}}
        codLig.at(tu.second + fatCorr.at(0)) = vlr;
    }
    if(module == 3){    
        for(auto tu:tabUso3){
            int vlr=0;
            for(auto tgd:tabGDef){if(tgd.first==tu.first){vlr=tgd.second; break;}}
            codLig.at(tu.second + fatCorr.at(0) + fatCorr.at(1)) = vlr;
        }
    }
    /*cout<<"CLIG-0--------------------------------"<<endl; 
    counter1=1; 
    for(auto a:codLig){cout<<"|"<<a<<"|"; if(counter1==4){cout<<endl;counter1=0;}; counter1++;}
    cout<<endl<<"--------------------------------------"<<endl;*/


    /*//CÓDIGO LIGADO ATUALIZADO
    cout<<"CLIG-2--------------------------------"<<endl; 
    counter=1; 
    for(auto a:codLig){cout<<"|"<<a<<"|"; if(counter==4){cout<<endl;counter=0;}; counter++;}
    cout<<endl<<"--------------------------------------"<<endl;*/

    //RESOLUÇÃO DOS ENDEREÇOS RELATIVOS
    counter=fatCorr.at(0);
    for(auto bit:bitMap.at(1)){
        if(bit == '1'){codLig.at(counter) += fatCorr.at(0);}
        counter++;
    }
    if(module == 3){
        for(auto bit:bitMap.at(2)){
            if(bit == '1'){codLig.at(counter) += fatCorr.at(0) + fatCorr.at(1);}
            counter++;
        }
    }
    //CÓDIGO LIGADO ATUALIZADO
    /*cout<<"CLIG-3--------------------------------"<<endl; 
    counter=1; 
    for(auto a:codLig){cout<<"|"<<a<<"|"; if(counter==4){cout<<endl;counter=0;}; counter++;}
    cout<<endl<<"--------------------------------------"<<endl;*/


}

void insertContentFile(string arq, int module){
//INSERINDO CODIGO OBJETO EM ARQUIVO .OBJ
    string newarq=arq;
    auto a = newarq.find(".mon");
    if(a!=-1){newarq.replace(a,4,".obj");}
    //cout<<endl<<"newarq->"<<newarq<<endl;
    
    ofstream MyFile(newarq);

    // Write to the file
    if(module == 1){
        int counter=1;
        for(auto linha:linhas){
            if(counter>=4){
                string Ters=linha;
                Ters.erase(Ters.begin(),Ters.begin()+2);
                MyFile<<Ters<<" ";
            }
            counter++;
        } 
    }
    else{
        for(auto cod:codLig){
            MyFile<<cod<<" ";
        }
    }
    
    // Close the file
    MyFile.close(); 
}


int main(int argc, char **argv){

    //VARIÁVEIS DA MAIN
    vector<string> cmdArguments(argv+1, argv + argc);
    string arq=argv[1];

    //VERIFICANDO A QUANTIDADE DE ARQUIVOS RECEBIDOS
    bool erro=false;
    if(cmdArguments.size() == 1){
        //cout<<"um arquivo"<<endl; 
        getFileContent(arq,linhas);
        insertContentFile(arq,1);
    }
    else if(cmdArguments.size() == 2){ 
        //cout<<"dois arquivos"<<endl; 
        getFileContent(arq,linhas); 
        getFileContent(argv[2],linhas2);
        exe(arq, 2);
        insertContentFile(arq,2);
    }
    else if(cmdArguments.size() == 3){
        //cout<<"tres arquivos"<<endl; 
        getFileContent(arq,linhas); 
        getFileContent(argv[2],linhas2);
        getFileContent(argv[3],linhas3);
        exe(arq, 3);
        insertContentFile(arq,3);

    }
    else{cout<<"NUMERO DE ARQUIVOS INDEVIDO"<<endl;}
    
    
    return 0;
}