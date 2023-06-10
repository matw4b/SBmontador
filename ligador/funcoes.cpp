#include "linkheader.h"

using namespace std;

vector<string> leitorDeArquivo(string arquivo){	//faz a leitura de todas as linhas do arquivo, salva em um vector retorna onde foi alocado
	vector<string> linhas;	//ao final, possui todas as linhas do arquivo
    string linha_atual;		//var auxiliar para ler a linha atual do arquivo
    ifstream arq_in(arquivo.c_str());
    while(!arq_in.eof()){
        getline(arq_in, linha_atual);
        linhas.push_back(linha_atual);
    }
	arq_in.close();
    return linhas;
}

vector<string> split_string(string linha){	//separa a label de um valor
	vector<string> separado;
	string token;
	stringstream ss (linha);
	while(ss >> token) separado.push_back(token);
	return separado;
}

//a funcao vai lendo as linhas e inserindo na tabela de uso ate encontrar um campo q seja de fora da tabela de uso (como DEF, por exemplo).
//observacao: alguns testes que encontrei no grupo da turma usam REF no lugar de RELATIVOS, o arquivo de exemplo no teams usa apenas RELATIVOS
// mas para conseguir rodar ambos, ajustei o if da linha de relativos.
void montarUso(vector<string> arq, vector<pair<string,int> > *tabelaUso){
	bool uso = false;
	string linha;
	vector<string> temp; //auxiliar para separar label de valor

	for(unsigned int i = 0; i < arq.size(); i++){
		linha = arq[i];
		if(linha.find("USO") != string :: npos){
			uso = true;
		}
		else if(linha.find("DEF") != string :: npos){
			uso = false;
		}
		else if((linha.find("RELATIVOS") != string :: npos) || (linha.find("REF") != string :: npos)){
			uso = false;
		}
		else if(linha.find("CODE") != string :: npos){
			uso = false;
		}
		else if(uso){
			temp = split_string(linha);
			if(temp.size() == 2)
				(*tabelaUso).push_back(make_pair(temp[0], atoi(temp[1].c_str())));
		}
	}
}

//a funcao vai lendo as linhas e inserindo na tabela de def ate encontrar um campo q seja de fora da tabela (como RELATIVOS , por exemplo)
void montarDef(vector<string> arq, vector<pair<string,int> > *tabelaDef){
	bool def = false;
	string linha;
	vector<string> temp; //auxiliar para separar label de valor

	for(unsigned int i = 0; i < arq.size(); i++){
		linha = arq[i];
		if(linha.find("USO") != string :: npos){
			def = false;
		}
		else if(linha.find("DEF") != string :: npos){
			def = true;
		}
		else if((linha.find("RELATIVOS") != string :: npos) || (linha.find("REF") != string :: npos)){
			def = false;
		}
		else if(linha.find("CODE") != string :: npos){
			def = false;
		}
		else if(def){
			temp = split_string(linha);
			if(temp.size() == 2)
				(*tabelaDef).push_back(make_pair(temp[0], atoi(temp[1].c_str())));
		}
	}
}

//monta a lista de enderecos a serem realocados (enderecos relativos)
void montarRelativos(vector<string> arq, vector<int> *enderecos){
	bool realoc = false;
	string linha;
	vector<string> temp; //auxiliar para separar um valor relativo do outro

	for(unsigned int i = 0; i < arq.size(); i++){
		linha = arq[i];
		if(linha.find("USO") != string :: npos){
			realoc = false;
		}
		else if(linha.find("DEF") != string :: npos){
			realoc = false;
		}
		else if((linha.find("RELATIVOS") != string :: npos) || (linha.find("REF") != string :: npos)){
			realoc = true;
		}
		else if(linha.find("CODE") != string :: npos){
			realoc = false;
		}
		else if(realoc){
			temp = split_string(linha);
			if(temp.size() > 0)
				for(unsigned int i = 0; i < temp.size(); i++){
					(*enderecos).push_back(atoi(temp[i].c_str()));
				}
			else
				realoc = false;
		}
	}
}

//pega o campo CODE do arquivo objeto
void montarCodigo(vector<string> arq, vector<int> *cod){
	bool code = false;
	string linha;
	vector<string> temp; //auxiliar para separar um valor do outro

	for(unsigned int i = 0; i < arq.size(); i++){
		linha = arq[i];
		if(linha.find("USO") != string :: npos){
			code = false;
		}
		else if(linha.find("DEF") != string :: npos){
			code = false;
		}
		else if((linha.find("RELATIVOS") != string :: npos) || (linha.find("REF") != string :: npos)){
			code = false;
		}
		else if(linha.find("CODE") != string :: npos){
			code = true;
		}
		else if(code){
			temp = split_string(linha);
			if(temp.size() > 0)
				for(unsigned int i = 0; i < temp.size(); i++){
					(*cod).push_back(atoi(temp[i].c_str()));
				}
			else
				code = false;
		}
	}
}

//monta a tabela de definicoes global utilizando as tabelas e fatores de cada modulo
void tabelaGlobal(vector<pair<string, int> > *def, int correcao){
	for(unsigned int i = 0; i < (*def).size(); i++){
		(tabelaDeDefGlob).push_back(make_pair((*def)[i].first, (*def)[i].second + correcao));
	}
}

// resolve as referencias cruzadas e corrige os enderecos relativos
void ligacao(vector<pair<string, int> > uso, vector<int> *code, vector<int> relativos, int correcao){
	
	bool usado = false;
	int j = 0;
	int k;
	int f = uso.size();

	for(unsigned int i = 0; i < relativos.size(); i++){
		k = 0;
		usado = false;
		j = 0;
		while(j < f){
			if(relativos[i] == uso[j].second){
				usado = true;
				break;
			}
			j++;
		}
		if(usado){
			while(uso[j].first != tabelaDeDefGlob[k].first)
				k++;
			(*code)[relativos[i]] = (*code)[relativos[i]] + tabelaDeDefGlob[k].second;
		}
		else{
			(*code)[relativos[i]] = (*code)[relativos[i]] + correcao;
		}
	}
}

//monta o arquivo executavel em uma unica linha
void executavel(vector<int> *codigo, string arquivo){
	ofstream saida;
	saida.open(arquivo.c_str());
	for(unsigned int i = 0; i < (*codigo).size(); i++){
		if(i < (*codigo).size() - 1)
			saida << (*codigo)[i] << " ";
		else
			saida << (*codigo)[i];
	}
	saida.close();
}