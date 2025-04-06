#pragma once
#include "data_structure.h"
void file_generation(char* filename, Row_data** head);
bool mach_line(Row_data* target_row, int Row_num);
void print_line(char* line, FILE* fp);
