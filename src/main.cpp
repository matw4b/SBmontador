#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../include/assembler.h"

using namespace std;

string toupper(string input){
	
	int i;
	for(i = 0; i < (int)input.length(); i++){
		input[i] = toupper(input[i]);
	}

	return input;
}

int main(int argc, char *argv[ ]){
	
	int i;
	string input_file_name;

	// Faz montagem dos arquivos
	for(i = 1; i < argc; i++)
	{
		input_file_name = argv[i];
		input_file_name.append(".asm");
		Assembler(input_file_name, i);   // Passa arquivo e seu numero identificador
	}
	

	return 0;
}
