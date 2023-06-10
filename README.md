# SBmontador
Trabalho 1 da disciplina de Software Básico
![Fluxograma do montador](/images/fluxograma.png)

# Como compilar e executar o compilador e o montador
Para o montador, basta colocar no terminal:

## gcc main.cpp assembler.cpp -ansi -Wall -lstdc++ -o montador

para executar:

## ./montador <arquivo>

Enquanto para o ligador:

## gcc funcoes.cpp ligador.cpp -ansi -Wall -lstdc++ -o linker

para executar:

## ./linker <arquivo> <arquivo> <arquivo> <arquivo>
 
Os arquivos devem estar ordenados para que o ligador possa ligar corretamente.
