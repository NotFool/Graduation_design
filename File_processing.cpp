#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "file_processing.h"
#include "data_structure.h"
#pragma warning(disable:4996) 

//�ж��ǲ������������
bool is_opera(char* s) {
	return (strcmp(s, "+") == 0 ||
		strcmp(s, "-") == 0 ||
		strcmp(s, "=") == 0 ||
		strcmp(s, "*") == 0 ||
		strcmp(s, "/") == 0 ||
		strcmp(s, "%") == 0 ||
		strcmp(s, "&") == 0 ||
		strcmp(s, "<") == 0 ||
		strcmp(s, ">") == 0 ||
		strcmp(s, ":") == 0 ||
		strcmp(s, "?") == 0 ||
		strcmp(s, "~") == 0 ||
		strcmp(s, ">=") == 0 ||
		strcmp(s, "<=") == 0);
}

//���д����ļ����ݣ���ȡ�����еĹؼ��֣�����ֵΪ��ǰ���ַ�������
int data_get(char* line,char strings[MAX_TOKENS][MAX_TOKEN_LENGTH]) {
	char* tokens[1000];
	char deliniters[] =" (), \n";
	int count = 0;

	//�Կո�Ϊ��λ��ȡ�ַ���
	char* token = strtok(line, deliniters);
	while (token != NULL) {
		tokens[count++] = token;
		strncpy(strings[count - 1], tokens[count - 1], MAX_TOKEN_LENGTH );
		token = strtok(NULL, deliniters);
	}

	return count;
}

//���д��ļ�
int file_get(char* filename, Row_data** head) {
	//���ļ����򿪴��󷵻�ֵΪ0
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("���ļ�����\n");
		return 0;
	}

	//�����������ʼ��λ��

	Row_data* prev = NULL;

	*head = InitRowdata();
	prev = *head;
	//����Ϊ��λ��ȡ�ļ�
	char line[1000];
	int Row_num = 0;
	while (fgets(line, sizeof(line), file) != NULL) {
		Row_num++;
		//printf("���ǵ�%d�У�����Ϊ��%s\n", Row_num, line);

		//�˴�Ӧ�ô�������������
		insertRowdata(&prev, Row_num);

		//����������ȡ
		int count;
		char strings[MAX_TOKENS][MAX_TOKEN_LENGTH];
		count = data_get(line,strings);

		//���ݷ��ಢ���д洢
		for (int i = 0; i < count; i++) {
			if (is_opera(strings[i])) {
				//printf("%s\n", strings[i]);
				char num=strings[i][0];
				//strcmp(&num, strings[i]);
				insertOpera(prev, num);
			}
			else {
				//printf("%s ", strings[i]);
				insertKeyword(prev, strings[i]);
			}
		}
	}

	//�ر��ļ���������������ֵΪ1
	fclose(file);
	return 1;
}