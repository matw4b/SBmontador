#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

string ToUpper(string input){
	
	int i;
	for(i = 0; i < input.length(); i++){
		input[i] = toupper(input[i]);
	}

	return input;
}

string DeleteComments(string input){

	int location = input.find(";");

	return input.substr(0, location);

}

string CleanSpaces(string input){
	
	bool is_on_start = true;
	string final;
	int i;
	int length = input.length();

	for(i = 0; i < length; i++)
	{

		if(input[i] == '\t'){		// Garante que vai remover todos os "tabs"
			continue;
		}
		else if(input[i] == ' ')	// Garante que vai remover todos os "espacos" desnecessarios
		{
			if(input[i+1] == ' ' || input[i+1] == '\t')
			{
				continue;
			}
			else
			{
				final += input[i];
			}
		}
		else{
			final += input[i];
		}

	}

	return final;
}

void PreProcessor(string source_code_file_name, int file_number){

	int length;
	string line, isolated_label, new_file_name;
	bool isolated_label_flag;
	ifstream source_code;
	ofstream processed_code;
	
	// Cria e nomeia arquivo para armazenar codigo pre processado
	new_file_name = source_code_file_name.substr(0, source_code_file_name.find(".asm"));
	new_file_name.append("_preprocessed.asm");

	// Abre arquivos
	processed_code.open(new_file_name);
	source_code.open(source_code_file_name);
	
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
				isolated_label = line;
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
					isolated_label_flag = false;
				}
			}
			
			// Insere linha pre processada no arquivo
			processed_code << line << endl;
             
		}
		
		source_code.close();
		processed_code.close();
	}
	else
	{
		cout << "ERRO: Não foi possível abrir o arquivo, ou ele não existe" << endl;
	}

}

#endif