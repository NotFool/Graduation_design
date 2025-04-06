#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data_structure.h"
#include "Character_storage.h"
#pragma warning(disable:4996) 

//��ӡ�У����ĳ������ļ��д�ӡ
void print_line(char* line,FILE* fp) {
	//printf("%s", line);
	fprintf(fp, "%s", line);
}

//ƥ���У��ҵ�Ŀ���ӡ��
bool mach_line(Row_data* target_row,int Row_num) {
	return ((target_row->row_num == Row_num) && target_row->flag == 1);
}

//������ɵ��ļ�
void file_generation(char* filename, Row_data** head) {
	FILE* file = fopen(filename, "r");
	int Row_num = 0;
	char line[1000];
	Row_data* target_row = (*head)->next;
	
	//��������ļ�
	char result_name[MAX_TOKEN_LENGTH];
	char Tail_affix[] = "_result";
	snprintf(result_name, sizeof(result_name), "%s%s.txt", filename, Tail_affix);
	FILE* fp = fopen(result_name, "w");
	if (fp == NULL) {
		printf("�����ļ�ʧ��");
		return;
	}

	//���ж�ȡ��ÿ�н��д���
	while (fgets(line, sizeof(line), file) != NULL) {
		Row_num++;
		if (mach_line(target_row, Row_num)) {
			print_line(line,fp);
		}
		target_row = target_row->next;
	}
}