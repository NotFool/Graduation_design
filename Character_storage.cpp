#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data_structure.h"
#include "file_processing.h"
#pragma warning(disable:4996)

void record_target_words(char** Target_words,char target[MAX_TOKEN_LENGTH],int num) {
	//�ж��Ƿ�Ŀ���ַ�����������
	if (num >= MAX_TOKENS) {
		printf("Ŀ���ַ���̫�࣡\n");
		return;
	}

	//�����ڴ���д洢
	size_t len = strlen(target);
	Target_words[num] = (char*)malloc((len+1) * sizeof(char));
	if (!Target_words[num]) {
		printf("Ŀ���ַ������ڴ�ʧ�ܣ�\n");
		return;
	}
	strcpy(Target_words[num], target);
}

void free_words(char** Target_words, int num) {
	for (int i = 0; i < num; i++) {
		free(Target_words[i]);
	}
}

void print_words(char** Target_words, int num) {
	printf("[");
	for (int i = 0; i < num; i++) {
		printf("%s ", Target_words[i]);
	}
	printf("]\n");
}