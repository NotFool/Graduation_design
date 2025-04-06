#pragma once
#include "data_structure.h"
#define MAX_TOKENS 1000
#define MAX_TOKEN_LENGTH 1000

bool is_opera(char* s);
int data_get(char* line, char strings[MAX_TOKENS][MAX_TOKEN_LENGTH]);
int file_get(char* filename, Row_data** head);
