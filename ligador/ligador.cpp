#include "linkheader.h"

using namespace std;

vector<pair<string,int> > tabelaDeDefGlob; //tabela de definicoes global

//o ligador liga os arquivos de acordo com a ordem colocada no terminal

int main(int argc, char* argv[])
{
	int i;
	vector<string> arq1; //guarda o modulo objeto todo
	vector<string> arq2;
	vector<string> arq3;
	vector<string> arq4;
	string aux1; //variavel que auxilia a montar o arquivo com o nome correto, concatenando o nome dele com o seu formato

	//tabelas e listas do mod1
	vector<pair<string,int> > tabelaUso1;
	vector<pair<string,int> > tabelaDef1;
	vector<int> enderecos1;
	vector<int> code1;
	int correcao1 = 0;

	//tabelas e listas do mod2
	vector<pair<string,int> > tabelaUso2;
	vector<pair<string,int> > tabelaDef2;
	vector<int> enderecos2;
	vector<int> code2;
	int correcao2 = 0;

	//tabelas e listas do mod3
	vector<pair<string,int> > tabelaUso3;
	vector<pair<string,int> > tabelaDef3;
	vector<int> enderecos3;
	vector<int> code3;
	int correcao3 = 0;

	//tabelas e listas do mod4
	vector<pair<string,int> > tabelaUso4;
	vector<pair<string,int> > tabelaDef4;
	vector<int> enderecos4;
	vector<int> code4;
	int correcao4 = 0;

	//como argc detecta o nome do ligador, ele conta 1 a mais do q a quantidade de arquivos fornecido
	//preciso checar se argc vale entre 3 e 5, ao inves de 2 e 4.
	if(argc < 3 || argc > 5){
		cout << "Quantidade invalida de argumentos" << endl;
	}
	else{
		for(i = 1;i < argc; i++){
			if(i == 1){// pega todas as informacoes do modulo 1
				aux1 = argv[1];
				aux1.append(".obj");
				arq1 = leitorDeArquivo(aux1);
				montarUso(arq1, &tabelaUso1);				montarDef(arq1, &tabelaDef1);
				montarRelativos(arq1, &enderecos1);
				montarCodigo(arq1, &code1);
				tabelaGlobal(&tabelaDef1, correcao1);
			}
			if(i == 2){
				aux1 = argv[2];
				aux1.append(".obj");
				arq2 = leitorDeArquivo(aux1);
				montarUso(arq2, &tabelaUso2);
				montarDef(arq2, &tabelaDef2);
				montarRelativos(arq2, &enderecos2);
				montarCodigo(arq2, &code2);
				correcao2 = code1.size();
				tabelaGlobal(&tabelaDef2, correcao2);
			}
			if(i == 3){
				aux1 = argv[3];
				aux1.append(".obj");
				arq3 = leitorDeArquivo(aux1);
				montarUso(arq3, &tabelaUso3);
				montarDef(arq3, &tabelaDef3);
				montarRelativos(arq3, &enderecos3);
				montarCodigo(arq3, &code3);
				correcao3 = code2.size() + correcao2;
				tabelaGlobal(&tabelaDef3, correcao3);
			}
			if(i == 4){
				aux1 = argv[4];
				aux1.append(".obj");
				arq4 = leitorDeArquivo(aux1);
				montarUso(arq4, &tabelaUso4);				
				montarDef(arq4, &tabelaDef4);
				montarRelativos(arq4, &enderecos4);
				montarCodigo(arq4, &code4);
				correcao4 = code4.size() + correcao3;
				tabelaGlobal(&tabelaDef4, correcao4);
			}
		}
	}

	if(argc == 3){
		ligacao(tabelaUso1, &code1, enderecos1, correcao1);
		ligacao(tabelaUso2, &code2, enderecos2, correcao2);
		for(unsigned int i = 0; i < code2.size(); i++)
			code1.push_back(code2[i]);
	}
	else if( argc == 4){
		ligacao(tabelaUso1, &code1, enderecos1, correcao1);
		ligacao(tabelaUso2, &code2, enderecos2, correcao2);
		ligacao(tabelaUso3, &code3, enderecos3, correcao3);
		for(unsigned int i = 0; i < code2.size(); i++)
			code1.push_back(code2[i]);
		for(unsigned int i = 0; i < code3.size(); i++)
			code1.push_back(code3[i]);
	}
	else if(argc == 5){
		ligacao(tabelaUso1, &code1, enderecos1, correcao1);
		ligacao(tabelaUso2, &code2, enderecos2, correcao2);
		ligacao(tabelaUso3, &code3, enderecos3, correcao3);
		ligacao(tabelaUso4, &code4, enderecos4, correcao4);
		for(unsigned int i = 0; i < code2.size(); i++)
			code1.push_back(code2[i]);
		for(unsigned int i = 0; i < code3.size(); i++)
			code1.push_back(code3[i]);
		for(unsigned int i = 0; i < code4.size(); i++)
			code1.push_back(code4[i]);
	}

	//faz o arquivo de saida
	aux1 = argv[1];
	aux1.append(".exc");
	executavel(&code1, aux1);

	//desaloca toda a memoria utilizada
	arq1.clear();
	arq2.clear();
	arq3.clear();
	arq4.clear();

	tabelaUso1.clear();
	tabelaDef1.clear();
	enderecos1.clear();
	code1.clear();

	tabelaUso2.clear();
	tabelaDef2.clear();
	enderecos2.clear();
	code2.clear();

	tabelaUso3.clear();
	tabelaDef3.clear();
	enderecos3.clear();
	code3.clear();

	tabelaUso4.clear();
	tabelaDef4.clear();
	enderecos4.clear();
	code4.clear();

	return 0;
}