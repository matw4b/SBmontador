# SBmontador
Trabalho 1 da disciplina de Software Básico: realizar um montador de passagem única e um ligador que funcione para até 4 arquivos. O fluxograma abaixo é do montador de uma passagem.
![Fluxograma do montador](/images/fluxograma.png)

## Alunos:

Matheus Guaraci Lima Bouças Alves 180046951

Luca Delpino Barbabella 180125559

# Como compilar e executar o compilador e o montador
Para o montador, basta colocar no terminal:

## gcc main.cpp assembler.cpp -ansi -Wall -lstdc++ -o montador

para executar:

## ./montador <arquivo>

Enquanto para o ligador:

## gcc funcoes.cpp ligador.cpp -ansi -Wall -lstdc++ -o linker

para executar:

## ./linker <arquivo> <arquivo> <arquivo> <arquivo>
 
Os arquivos devem estar ordenados para que o ligador possa funcionar corretamente.
 
# Sistema operacional utilizado:
linux mint 21 vanessa e ubuntu 22.04 lts
