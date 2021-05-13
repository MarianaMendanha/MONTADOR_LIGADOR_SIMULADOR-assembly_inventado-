# MONTADOR LIGADOR SIMULADOR -assembly inventado-
Trabalho 2 do Curso de Software Básico com montador para módulos, ligador e simulador de assembly inventado 

## 🚧 STATUS DO PROJETO
- [x] Montador de 1 a 3 arquivos .asm
- [x] Ligador de 1 a 3 arquivos .mon
- [ ] Simulador_proprio de entrada arquivo.obj (implementado mas com erros de SegFault)

## Primeiros Passos

Para executar o projeto, é interessante instalar os seguintes programas:

- [VSCode: Recomendado para executar o projeto C++](https://code.visualstudio.com/download)
- [Extensão C\C++: Recomendado para realizar o build do projeto C++](https://code.visualstudio.com/download)

## Sobre o Assembly inventado
As instruções utilizadas no assembly inventado podem ser vistas a seguir:
![6 - CONJUNTO DE INSTRUÇÕES DA MÁQUINA HIPOTÉTICA](https://user-images.githubusercontent.com/72531807/113185256-9fac0800-922c-11eb-84fd-fa2b32eed7ab.png)
Sendo que para uma entrada de mais de um arquivo deve-se ter as diretivas ```BEGIN``` e ```END``` e para variáveis públicas (de uso em outros módulos a não ser o módulo declarado) devem ser declaradas como ```PUBLIC``` e variáveis externas (declaradas em outros módulos) devem ser declaradas como ```EXTERN``` como pode ser visto em exemplo de código na seção MONTADOR.

## 💻 Compilando
***A IMPLEMENTAÇÃO DO TRABALHO FOI FEITA COM O USO DO VSCODE E DA LINGUAGEM C++ NO WINDOWS***   

***O SIMULADOR UTILIZADO FOI FORNECIDO PELO PROFESSOR***

### 🛠 1.  MONTADOR
O arquivo montador.cpp deve ser compilado e executado para que o montador seja utilizado. Para montar, deve-se acessar a pasta do diretório de onde se encontra o arquivo .cpp utilizando o comando: 
```cd "seu diretório aqui"``` 
e em seguida para compilar o montador:
```"g++ montador.cpp -o montador"``` 
e então utilizando de 1 a 3 arquivos .asm já existentes (código com a linguagem hipotética) com o arquivo de entrada na linha de comando no formato: 
```".\montador.exe myprogram1.asm myprogram2.asm"``` 
para a execução do programa. Caso o código .asm esteja escrito de acordo com as especificações, que são: section data antes de text, não ter BEGIN e END no caso de recebimento de apenas 1 arquivo, ter BEGIN e END no caso de 2 ou 3 arquivos e as variáveis do tipo PUBLIC e EXTERN estarem dentro da SECTION DATA como o exemplo:
``` asm
MOD_A: BEGIN
SECTION DATA
    MOD_B: EXTERN
	  PUBLIC FT
	  PUBLIC N
	  N: SPACE
	  ONE: CONST 1
SECTION TEXT
    INPUT N
    LOAD N
    FT: SUB ONE
    JMPZ FIM
    JMP MOD_B
    FIM: OUTPUT N
    STOP
END
```
***IMPORTANTE FRIZAR QUE APESAR DO CÓDIGO TER TRATAMENTO DE TAB ("\r") NA REVISÃO DO PROFESSOR O PROGRAMA TEVE PROBLEMAS COM ISSO (NÃO SEI PQ) ENTÃO CASO ACONTEÇA O MESMO, TESTE NOVAMENTE SEM TABS NO ARQUIVO ENTRADA.*** Arquivos são gerados de mesmo nome de seu arquivo entrada (com a extensão .mon) com o código objeto para cada arquivo com as mesmas especificações de formato (nome, tamanho, mapa de bits, tabela de uso, tabela de definições e código objeto), como pode ser visto no exemplo a seguir:
```
H NomeDoArquivo
H 15
R 010101010001000
U MOD_B 9
D FAT 4
D N 13
T 12 13
T 10 13
T 2 14
T 8 10
T 5 0
T 13 13
T 14
T 0
T 1
```
caso o código .asm não esteja escrito de acordo com as especificações passadas, não é gerado arquivo de saída .mon. 
 

### ⚙️ 2.  LIGADOR
De mesma forma, O arquivo ligador.cpp deve ser compilado e executado para que o montador seja utilizado. Para montar, deve-se acessar a pasta do diretório de onde se encontra o arquivo .cpp utilizando o comando cd e em seguida digitar ```"g++ ligador.cpp -o ligador"``` e então utilizando o(s) arquivo(s) .obj gerado(s) anteriormente (código objeto no formato .mon do .asm escrito corretamente) como arquivo de entrada na linha de comando no formato ```".\ligador myprogram1.obj myprogram2.obj"``` gera-se um arquivo de mesmo nome de extensão .obj com o código objeto final já ligado dos módulos, como mostrado a seguir:
```
12 13 10 13 2 14 8 10 5 15 13 13 14 0 1 11 25 3 13 11 13 10 25 5 4 0 
```
  
### 🚀 3.  SIMULADOR
Para a execução deste código gerado pelo ligador é utilizado o simulador fornecido pelo professor a partir do comando ```".\simulador.exe myprogram1.obj"``` (já no diretório do executável) e assim o programa ja ligado é executado. O simulador_proprio foi implementado mas no momento tem erros.

## 💜 Contribuições
Contribuições são sempre bem-vindas! Para contribuir com testes lembre-se sempre de adiciona-los com a devida documentação, seja arquivos de entrada para o montador, ligador ou simulador.

## 💡 Testes
Os arquivos de entrada para o montador se encontram na pasta MONTADOR_tests, os do ligador em LIGADOR_tests e do simulador em SIMULADOR_tests as entradas e saídas obtidas para montador com erros se encontra em MONTADOR_error.
