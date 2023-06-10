#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>

#include "../include/assembler.h"

using namespace std;

map <string, string> OPCode; 
map <string, int> Diretivas;

// Atualmente só há uma tabela dessas abaixo, fazer com que exista uma tabela dessas para cada arquivo

vector < pair<string, vector<int> > > SymbolsTable;
vector < pair<string, int> > DefinitionsTable;
vector < pair<string, int> > UseTable;
vector < pair<string, int> > relativos;
vector<vector<string> > decomposed_instructions_list;

void StartTables(){

	// Instrucoes
	OPCode["ADD"] =  "1";
	OPCode["SUB"] =  "2";
	OPCode["MUL"] =  "3";
	OPCode["DIV"] =  "4";
	OPCode["JMP"] =  "5";
	OPCode["JMPN"] =  "6";
	OPCode["JMPP"] =  "7";
	OPCode["JMPZ"] =  "8";
	OPCode["COPY"] =  "9";
	OPCode["LOAD"] =  "10";
	OPCode["STORE"] =  "11";
	OPCode["INPUT"] =  "12";
	OPCode["OUTPUT"] =  "13";
	OPCode["STOP"] =  "14";

	// Diretivas
	Diretivas["SPACE"] = 1; 
    Diretivas["CONST"] = 1; 
    Diretivas["BEGIN"] = 0;		// To devendo
    Diretivas["END"] = 0;		// To devendo
    // Diretivas["EXTERN"] = 0;	Já lido com ela diretamente
    // Diretivas["PUBLIC"] = 0;	Já lido com ela diretamente

}

string ToUpper(string input){
	
	int i;
	for(i = 0; i < (int)input.length(); i++){
		input[i] = toupper(input[i]);
	}

	return input;
}

string DeleteComments(string input){

	int location = input.find(";");

	return input.substr(0, location);

}

string CleanSpaces(string input){
	
	string final;
	int i;
	int length = input.length();
	bool started = false;
	bool is_removable = false;

	for(i = 0; i < length; i++)
	{

		if(input[i] == ' ' || input[i] == '\t'){
			is_removable = true;
			continue;
		}
		else{
			
			if(is_removable){

				if(!started){
					final += input[i];
					started = true;
				}
				else{
					final += " ";
					final += input[i];
				}
				is_removable = false;
			}
			else{
				final += input[i];
				started = true;
			}

		}

	}

	return final;
}

void PreProcessor(string source_code_file_name, int file_number){

	string line, isolated_label, new_file_name;
	bool isolated_label_flag = false;
	ifstream source_code;
	ofstream processed_code;

	vector<string> section_data;
	vector<string> decomposed_line;

	// Cria e nomeia arquivo para armazenar codigo pre processado
	new_file_name = source_code_file_name.substr(0, source_code_file_name.find(".asm"));
	new_file_name.append("_preprocessed.asm");

	// Abre arquivos
	processed_code.open(new_file_name.c_str());
	source_code.open(source_code_file_name.c_str());

	// Comeca processamento do codigo fonte
	if(source_code.is_open() && source_code.is_open())
	{

		// faz iteração sobre todas as linhas do código fonte
		while (!source_code.eof())
		{

			getline (source_code, line);        
			line = ToUpper(line);       // torna tudo maiúsculo

			// Deleta comentários
			line = DeleteComments(line);
			// Deleta espaços desnecessarios
			line = CleanSpaces(line);

			// Pula para próxima iteracao caso haja somente uma quebra de linha
			if(line.length() == 0)
			{
				continue;
			}

			// Verifica se somente a label está presente na linha
			if(line[line.length()-1] == ':')
			{
				isolated_label.append(line);
				isolated_label += ' ';
				isolated_label_flag = true;
				continue;
			}
			// Se a flag de label isolada for true, concatena com a próxima linha
			else
			{
				if(isolated_label_flag)
				{
					line = isolated_label.append(line);
					isolated_label.clear();
					isolated_label_flag = false;
					
				}
			}
			if(line.find_last_of(":") != line.find(":"))
			{
					cout << "ERRO: MAIS DE UMA LABEL NA MESMA LINHA: '" << line << "'" << endl;
			}
			if(line.find("CONST") != std::string::npos || line.find("SPACE") != std::string::npos)	// Guarda informacoes do section data para serem armazenados no arquivo depois
			{
				section_data.push_back(line);
				continue;
			}

			line.append(" ");
			// Insere linha pre processada no arquivo
			processed_code << line << endl;
             
		}
		for(int i = 0; i < (int)section_data.size(); i++)		// Adiciona dados do section data no final do arquivo
		{
			processed_code << section_data[i] << endl;
		}
		
		source_code.close();
		processed_code.close();
	}
	else
	{
		cout << "ERRO: Não foi possível abrir o arquivo do código fonte original, ou ele não existe" << endl;
	}

}

// Verifica se as labels utilizadas ja foram declaradas
int check_if_declared_in_relativos(string input)
{
	int i;

	for(i = 0; i < (int)relativos.size(); i++)
	{
		if(relativos[i].first == input)
		{
			return relativos[i].second;
		}
	}

	return -1;
}

int check_if_in_symbols_table(string(input))
{
	int i;

	for(i = 0; i < (int)SymbolsTable.size(); i++)
	{
		if(SymbolsTable[i].first == input)
		{
			return i;
		}
	}

	return -1;
}

// Pega uma string e divide em substrings separadas pelo espaço (do input)
vector<string> OperationDecomposer(string input)
{
	string delimiter = " ", aux;
	int start = 0;
	int end = input.find(delimiter);
	vector<string> return_vector;

	if(input.find(",") != std::string::npos)
	{
		replace(input.begin(), input.end(), ',', ' ');
		input = CleanSpaces(input);
		input.append(" ");
		// cout << input << endl;		
	}


	while (end != -1) {
        
		aux = input.substr(start, end - start);

		if(aux != " ")
		{
			return_vector.push_back(aux);
		}
		
		start = end + delimiter.size();
        end = input.find(delimiter, start);
    }

	// if(input.find("COPY") != std::string::npos){

	// 	cout << input << endl;
	// 	for (int i = 0; i < (int)return_vector.size(); i++)
	// 	{
	// 		cout << return_vector[i] << " ";
	// 	}
	// 	cout << endl;
	// }
	

	// Nao coloca "," no caso: COPY A , B. Então o vector de retorno tem tamanho 3

	return return_vector;

}

void substitute_in_decomposed_code(int input, string label)
{

	bool changed_value = false;
	int i, adicional_value = 0;
	ostringstream aux;
	stringstream stoi;
	string new_value;

	for (int i = 0; i < (int)decomposed_instructions_list[input].size(); i++)		// Percorre instrucao e identifica se tem soma. Se tiver, guarda o valor a ser adicionado
	{		
		if(decomposed_instructions_list[input][i].find("+") != std::string::npos)
		{
			// cout << "INSTRUCAO COM +:" << endl;
			// for (int j = 0; j < (int)decomposed_instructions_list[input].size(); j++)
			// {
			// 	cout << decomposed_instructions_list[input][j] << " ";
			// }
			// cout << endl;
			if(decomposed_instructions_list[input][i+1].length() >= 2){

					cout << "YOOOOOOOOOOOOOOOOO " << decomposed_instructions_list[input][i+1][1] << endl;
				if(decomposed_instructions_list[input][i+1][0] == '0' && decomposed_instructions_list[input][i+1][1] == 'X')
				{

					std::stringstream ss;
					ss << std::hex << decomposed_instructions_list[input][i+1];
					ss >> adicional_value;

					adicional_value = adicional_value;
					std::cout << "CONVERTIDO: " << adicional_value << std::endl;
				}
				else
				{
					stoi << decomposed_instructions_list[input][i+1];
					stoi >> adicional_value;		
				}
			}
			else{

				stoi << decomposed_instructions_list[input][i+1];
				stoi >> adicional_value;
			}

			decomposed_instructions_list[input].erase(decomposed_instructions_list[input].end() - 1);
			decomposed_instructions_list[input].erase(decomposed_instructions_list[input].end() - 1);

			// cout << "DEPOIS DO AJUSTE:" << endl;
			// for (int j = 0; j < (int)decomposed_instructions_list[input].size(); j++)
			// {
			// 	cout << decomposed_instructions_list[input][j] << " ";
			// }

			// decomposed_instructions_list[input][j].resize(decomposed_instructions_list[input][j].size()	 - 2);		// Remove dois ultimos membros do vector ('+' e o 'numero a ser adicionado')

			break;
		}
	}

	for(i = 0; i < (int)relativos.size(); i++)
	{
		if(relativos[i].first == label)
		{

			aux << relativos[i].second + adicional_value;
			new_value = aux.str();
			changed_value = true;
			break;
		}
	}

	if(!changed_value)		// Necessario para manter nome de labels que nao foram declaradas em SECTION DATA
	{
		new_value = label;
	}

	for (i = 0; i < (int)decomposed_instructions_list[input].size(); i++)
	{

		if(decomposed_instructions_list[input][i] == label)
		{
			decomposed_instructions_list[input][i] = new_value;
		}

	}

}

void Assembler(string source_code_file_name, int file_number){

	int line_counter, i, j, lastpos;
	string line, aux;
	string   preprocessed_file_name;
	ifstream preprocessed_file;
	
	vector<string> extern_labels;	// Usado na identificacao do uso de labels externas para montar tabela de uso
	vector<string> first_translation_pass;
	vector<string> decomposed_instruction;
	vector<string> section_data;
	vector<string> final_code;
	
	bool section_text_existence = false;
	bool section_data_existence = false;
	bool just_found_extern;
	bool add_line_to_final = true;
	
	bool has_modules = false;		// Usado para verificar qual tipo de arquivo de saída sera gerado
	string output_file_name;
	ofstream output_file;

	StartTables();
	PreProcessor(source_code_file_name, file_number);

	preprocessed_file_name = source_code_file_name.substr(0, source_code_file_name.find(".asm"));
	preprocessed_file_name.append("_preprocessed.asm");

	preprocessed_file.open(preprocessed_file_name.c_str());


	if(preprocessed_file.is_open())
	{
		line_counter = 0;
		while (!preprocessed_file.eof())
		{

			getline(preprocessed_file, line);

			if(line.find("BEGIN") != std::string::npos)
			{
				DefinitionsTable.push_back(make_pair(line.substr(0, line.find(":")), 0));
				add_line_to_final = false;
				has_modules = true;
			}
			else if(line.find("SECTION TEXT") != std::string::npos )
			{
				section_text_existence = true;
			}

			else if(line.find("SECTION DATA") != std::string::npos )
			{
				section_data_existence = true;
				continue;
			}
			else if(line.find("END") != std::string::npos )
			{
				continue;
			}

			// cout << "linha analisada: " << line << endl;
			// if(line.find_last_of(":") != line.find(":"))
			// {
			// 	cout << "ERRO: MAIS DE UMA LABEL NA MESMA LINHA" << endl;		// NAO ESTA FUNCIONANDO, ARRUMAR DETECCAO DESSE ERRO
			// }

			// Bota na tabela de relativos se achar label nova que não seja extern
			if(line.find(":") != std::string::npos)
			{
				
				if((line.find("EXTERN") == std::string::npos))	// Coloca na tabela de relativos se for uma label comum
				{
					relativos.push_back( make_pair(line.substr(0, line.find(":")), line_counter) );
				}
				else	// Cria referência para adicionar na tabela de uso quando for utilizado
				{
					if(!has_modules)
					{
						cout << "ERRO: EXISTENCIA DE 'EXTERN' EM ARQUIVO QUE NAO POSSUI BEGIN" << endl;
					}

					relativos.push_back( make_pair(line.substr(0, line.find(":")), line_counter) );
					add_line_to_final = false;
					extern_labels.push_back( CleanSpaces( line.substr(0, line.find("EXTERN")-2) ) );
					just_found_extern = true;
				}
			}
			// Coloca na tabela de definicoes quando for necessario
			else if(line.find("PUBLIC") != std::string::npos)
			{
				if(!has_modules)
				{
					cout << "ERRO: EXISTENCIA DE 'PUBLIC' EM ARQUIVO QUE NAO POSSUI BEGIN" << endl;
				}

				add_line_to_final = false;
				aux = CleanSpaces(line.substr(6, line.length()));			// 6 é parametro pois o elemento de indice 6 na linha eh o espaco logo apos "PUBLIC"
				DefinitionsTable.push_back(make_pair(aux, line_counter));
			}

			// Checa se na linha atual há algum uso de label externa
			for(i = 0; i < (int)extern_labels.size(); i++)
			{
				// Se encontrar o uso de alguma label externa, adiciona na tabela de usos
				if(line.find(extern_labels[i]) != std::string::npos && !just_found_extern)
				{
					UseTable.push_back(make_pair(extern_labels[i], line_counter));
					just_found_extern = false;
				}
			}
			just_found_extern = false;

			// TRADUZ DE ASSEMBLY PARA CODIGO DE MAQUINA E ARMAZENA EM UM VECTOR DE STRINGS. DEPOIS DE ISSO SER FEITO, RESOLVE REFERENCIAS PENDENTES E BOTA TUDO NUMA LINHA SO EM UM NOVO ARQUIVO

			aux = line;
			if(line.find(":") != std::string::npos)		// Se encontra label na linha, faz tratamento para armazenar em aux tudo após label
			{

				lastpos = line.find_last_of(":");

				if(line.substr(0,lastpos)[0] >= '0' && line.substr(0,lastpos)[0] <= '9')
				{
					cout << "ERRO LEXICO DETECTADO: USO DO NUMERO '" << line.substr(0,lastpos)[0] << "' NO INICIO DO NOME DA LABEL '" << line.substr(0,lastpos) << "'" << endl;
				}

				for(i = 0; i < (int)line.substr(0, lastpos).size(); i++)
				{
					if( line.substr(0,lastpos)[i] != '_' && ((line.substr(0,lastpos)[i] < 'A' || line.substr(0,lastpos)[i] > 'Z') &&
															 (line.substr(0,lastpos)[i]  < '0' || line.substr(0,lastpos)[i] > '9')) )
					{
						cout << "ERRO LEXICO DETECTADO: USO DO CARACTERE ESPECIAL '" << line.substr(0,lastpos)[i] << "' NO NOME DA LABEL '" << line.substr(0,lastpos) << "'" << endl;
					}
				}

				// if(check_if_declared_in_relativos(aux.substr(0, aux.find(":")))){
				// 	// cout << "lol " << aux.substr(0, aux.find(":")) << endl;
				// 	// relativos.push_back( make_pair(line.substr(0, lastpos), line_counter) );	// Adiciona label a lista de relativos
				// }

				aux = CleanSpaces(line.substr(lastpos+1, line.length()));
				aux.append(" ");

			}

			decomposed_instruction = OperationDecomposer(aux);

			vector<int> new_reference_list;
			ostringstream str1;
			int declared_position;
			aux = "";

			// Vai direto se for OPCode
			if(OPCode.count(decomposed_instruction[0]))
			{
				aux.append( OPCode[decomposed_instruction[0]] );
				aux.append(" ");

				for(i = 1; i < (int)decomposed_instruction.size(); i++)		// Verifica se as labels estão declaradas e age de acordo
				{
					declared_position = check_if_declared_in_relativos(decomposed_instruction[i]);
					if(declared_position != -1)	// SE ESTIVER EM relativos, JA COLOCA EM first_translation_pass COM A REFERENCIA CERTA
					{
						str1 << declared_position;
						aux.append(str1.str());
						aux.append(" ");
					}
					else	// Se ainda nao foi declarado, coloca apenas o nome da label e adiciona referencia em SymbolsTable
					{
						aux.append(decomposed_instruction[i]);
						aux.append(" ");

						declared_position = check_if_in_symbols_table(decomposed_instruction[i]);
						if(declared_position == -1)		// Se ainda nao exta em SymbolsTable, cria nova entrada
						{
							new_reference_list.clear();
							new_reference_list.push_back(line_counter);
							SymbolsTable.push_back(make_pair( decomposed_instruction[i], new_reference_list ));

							// aux.append();
						}
						else	// Se ja estiver em SymbolsTable, apenas adiciona nova referencia
						{
							SymbolsTable[declared_position].second.push_back(line_counter);
						}
					}

				}

			}
			// CHECA SE ESTA NO MAPA DE DIRETIVAS
			else if(Diretivas.count(decomposed_instruction[0]))
			{

				if(decomposed_instruction[0] == "CONST")
				{
					aux.append(decomposed_instruction[1]);
					aux.append(" ");
				}
				else if(decomposed_instruction[0] == "SPACE")
				{
					// cout << line << " 00" << endl;
					aux.append("00 ");					
				}

			}


				// cout << "ZE DA MANGA: " << aux << endl;

				if(add_line_to_final){
					
					// if(Diretivas.count(decomposed_instruction[0]))
					// {
						// section_data.push_back(aux);
					// }
					// else
					// {
					// }
					first_translation_pass.push_back(aux);		// Finalmente adiciona instrucao traduzida em first_translation_pass
					decomposed_instructions_list.push_back(OperationDecomposer(aux));			
					line_counter++;

				}
				
				
				add_line_to_final = true;
				
				

				

			//  ESTOU GUARDANDO DECLARACOES DE LABELS EM "relativos". SymbolsTable VAI SERVIR PARA ARMAZENAR LISTA DE REFERENCIAS NAO RESOLVIDAS

		}

		// Lancamento de erro caso nao exista section text
		if(!section_text_existence)
		{
			cout << "ERRO SEMÂNTICO DETECTADO: NÃO FOI DEFINIDO UM SECTION TEXT NO ARQUIVO DE ENTRADA" << endl;
		}

		// Lancamento de erro caso nao exista section data
		if(!section_data_existence)
		{
			cout << "ERRO SEMÂNTICO DETECTADO: NÃO FOI DEFINIDO UM SECTION DATA NO ARQUIVO DE ENTRADA" << endl;
		}


		// Verifica se foram usados caracteres especiais no nome de alguma label

		cout << "lol" << endl;
		for(i = 0; i < (int)decomposed_instructions_list.size(); i++)
		{

			for(j = 0; j < (int)decomposed_instructions_list[i].size(); j++)
			{
				cout << decomposed_instructions_list[i][j] << " ";
			}
			cout << endl;

		}




		// RESOLVE REFERENCIAS PENDENTES
		
		vector<int> symbols_table_linked_list;
		for (i = 0; i < (int)SymbolsTable.size(); i++)
		{

			symbols_table_linked_list = SymbolsTable[i].second;
			for (j = 0; j < (int)symbols_table_linked_list.size(); j++){

				substitute_in_decomposed_code(symbols_table_linked_list[j], SymbolsTable[i].first);
			}

		}

		// Resolve referencias de tabela de definicoes

		for(i = 0; i < (int)DefinitionsTable.size(); i++)
		{

			for(j = 0; j < (int)relativos.size(); j++)
			{
				if(DefinitionsTable[i].first.find(relativos[j].first) != std::string::npos)
				{
					DefinitionsTable[i].second = relativos[j].second;
					break;
				}
			}

		}


		preprocessed_file.close();
	}
	else
	{
		cout << "ERRO: NÃO FOI POSSÍVEL ABRIR O ARQUIVO PRÉ PROCESSADO PARA O PROCESSO DE MONTAGEM, OU ELE NÃO EXISTE" << endl;
	}

	// Verifica se alguma label nao declarada foi usada em text

	for(i = 0; i < (int)decomposed_instructions_list.size(); i++)
	{
		for(int j = 0; j < (int)decomposed_instructions_list[i].size(); j++)
		{
			if(decomposed_instructions_list[i][j][0] < '0' || decomposed_instructions_list[i][j][0] > '9')
				cout << "ERRO: A LABEL '" << decomposed_instructions_list[i][j] << "' NAO FOI DECLARADA EM SECTION DATA" << endl;
		}
		cout << endl;
	}

	// Cria arquivo de saida

	output_file_name = source_code_file_name.substr(0, source_code_file_name.find(".asm"));

	if(has_modules)		// Caso tenha modulos, cria obj
		output_file_name.append(".obj");
	else				// Caso nao tenha modulos, cria exc
		output_file_name.append(".exc");

	output_file.open(output_file_name.c_str());

	if(output_file.is_open())
	{

		if(has_modules)		// Cria arquivo .obj e coloca tabela de definicoes, de uso, relativos e codigo
		{
			output_file << "USO\n";
			for (i = 0; i < (int)UseTable.size(); i++)
			{
				output_file << UseTable[i].first << " " << UseTable[i].second << "\n";
			}

			output_file << "DEF\n";
			for (i = 0; i < (int)DefinitionsTable.size(); i++)
			{
				output_file << DefinitionsTable[i].first << " " << DefinitionsTable[i].second << "\n";
			}

			output_file << "RELATIVOS\n";
			for (i = 0; i < (int)relativos.size(); i++)
			{
				output_file << relativos[i].second << " ";
			}
			output_file << "\n";

			output_file << "CODE\n";
			for (i = 0; i < (int)decomposed_instructions_list.size(); i++)
			{
				for (j = 0; j < (int)decomposed_instructions_list[i].size(); j++)
				{
					output_file << decomposed_instructions_list[i][j] << " ";
				}
				
			}

		}
		else	// Cria arquivo .exc e coloca somente as instrucoes
		{
			for (i = 0; i < (int)decomposed_instructions_list.size(); i++)
			{
				for (j = 0; j < (int)decomposed_instructions_list[i].size(); j++)
				{
					output_file << decomposed_instructions_list[i][j] << " ";
				}
				
			}
			

		}



			

			output_file.close();

	}



	// LOOPS USADOS PARA DEBUGGING

	for(i = 0; i < (int)first_translation_pass.size(); i++)
	{
		cout << first_translation_pass[i] << endl;
	}
	cout << endl;

	for(i = 0; i < (int)decomposed_instructions_list.size(); i++)
	{
		for(int j = 0; j < (int)decomposed_instructions_list[i].size(); j++)
		{
			cout << decomposed_instructions_list[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;

	cout << "DefinitionsTable: " << endl;
	for(i = 0; i < (int)DefinitionsTable.size(); i++){
		cout << DefinitionsTable[i].first << " " << DefinitionsTable[i].second << endl;
	}
	cout << endl;

	cout << "relativos: " << endl;
	for(i = 0; i < (int)relativos.size(); i++){
		cout << relativos[i].first << " " << relativos[i].second << endl;
	}
	cout << endl;

	cout << "UseTable: " << endl;
	for(i = 0; i < (int)UseTable.size(); i++){
		cout << UseTable[i].first << " " << UseTable[i].second << endl;
	}
	cout << endl;

	cout << "ExternLabels: " << endl;
	for(i = 0; i < (int)extern_labels.size(); i++){
		cout << extern_labels[i] << endl;
	}
	cout << endl;

	cout << "TAMANHO DA decomposed_instructions_list: ";
	cout << decomposed_instructions_list.size() << endl;

}



