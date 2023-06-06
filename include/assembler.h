#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

string ToUpper(string input);

string DeleteComments(string input);

string CleanSpaces(string input);

void PreProcessor(string source_code_file_name, int file_number);

void Assembler(string source_code_file_name, int file_number);

#endif