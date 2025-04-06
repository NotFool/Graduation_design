#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data_structure.h"
#include "Character_storage.h"
#pragma warning(disable:4996) 

//打印行，最后改成生成文件中打印
void print_line(char* line,FILE* fp) {
	//printf("%s", line);
	fprintf(fp, "%s", line);
}

//匹配行，找到目标打印行
bool mach_line(Row_data* target_row,int Row_num) {
	return ((target_row->row_num == Row_num) && target_row->flag == 1);
}

//最后生成的文件
void file_generation(char* filename, Row_data** head) {
	FILE* file = fopen(filename, "r");
	int Row_num = 0;
	char line[1000];
	Row_data* target_row = (*head)->next;
	
	//生成输出文件
	char result_name[MAX_TOKEN_LENGTH];
	char Tail_affix[] = "_result";
	snprintf(result_name, sizeof(result_name), "%s%s.txt", filename, Tail_affix);
	FILE* fp = fopen(result_name, "w");
	if (fp == NULL) {
		printf("生成文件失败");
		return;
	}

	//按行读取，每行进行处理
	while (fgets(line, sizeof(line), file) != NULL) {
		Row_num++;
		if (mach_line(target_row, Row_num)) {
			print_line(line,fp);
		}
		target_row = target_row->next;
	}
}