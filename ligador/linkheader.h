#ifndef LINKHEADER_H
#define LINKHEADER_H

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <utility>

using namespace std;

//variaveis
extern vector<pair<string,int> > tabelaDeDefGlob; //tabela de definicoes global

//funcoes
vector<string> leitorDeArquivo(string arquivo);

void montarUso(vector<string> arq, vector<pair<string,int> > *tabelaUso);

void montarDef(vector<string> arq, vector<pair<string,int> > *tabelaDef);

void montarRelativos(vector<string> arq, vector<int> *enderecos);

void montarCodigo(vector<string> arq, vector<int> *cod);

void tabelaGlobal(vector<pair<string, int> > *def, int correcao);

void ligacao(vector<pair<string, int> > uso, vector<int> *code, vector<int> relativos, int correcao);

void executavel(vector<int> *codigo, string arquivo);

#endif