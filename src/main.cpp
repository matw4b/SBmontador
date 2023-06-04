#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../include/preprocessor.h"

using namespace std;

string toupper(string input){
	
	int i;
	for(i = 0; i < input.length(); i++){
		input[i] = toupper(input[i]);
	}

	return input;
}

int main(int argc, char *argv[ ]){
	
	int i;
	
	// Faz pre processamento dos arquivos
	for(i = 1; i < argc; i++)
	{
		PreProcessor(argv[i], i);   // Passa arquivo e seu numero identificador
	}
	

	return 0;
}
