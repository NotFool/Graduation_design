#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data_structure.h"
#include "file_processing.h"
#pragma warning(disable:4996)

void record_target_words(char** Target_words,char target[MAX_TOKEN_LENGTH],int num) {
	//判断是否目标字符串数量超限
	if (num >= MAX_TOKENS) {
		printf("目标字符串太多！\n");
		return;
	}

	//分配内存进行存储
	size_t len = strlen(target);
	Target_words[num] = (char*)malloc((len+1) * sizeof(char));
	if (!Target_words[num]) {
		printf("目标字符分配内存失败！\n");
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