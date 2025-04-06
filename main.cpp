#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data_structure.h"
#include "file_processing.h"
#include "Character_storage.h"
#include "string_matching.h"
#include "Result_generation.h"
#pragma warning(disable:4996) 


//�����ļ���ȡ���ܴ���
int main() {
	//��ȡ�ļ�λ��
	char string[100];
	scanf("%s", &string);
	int flag;
	
	//�����ļ����������ṹ
	Row_data* head;
	head = NULL;
	flag = file_get(string,&head);
	if (!flag) {
		printf("��������ȷ���ļ���ַ��\n");
		return 0;
	}
	
	//��ȡ��Ҫ����ƥ����ַ���
	char target_word[MAX_TOKEN_LENGTH];
	char* Target_words[MAX_TOKENS];
	scanf("%s", target_word);
	char stop[] = "NULL";
	int num = 0;
	while (strcmp(target_word,stop) != 0) {
		//printf("%s\n", target_word);
		record_target_words(Target_words,target_word,num++);
		scanf("%s", target_word);
	}
	//print_words(Target_words, num);

	//�����Ա��ַ����ؼ���������жԱȣ�ƥ��ɹ��޸�flag
	int count = 0;
	while (count < num) {
		keyword_ergodic(head,Target_words,count++,&num);
	}

	free_words(Target_words, num);
	
	//��ӡ����ṹ
	//printRow(head);
	
	// ��ӡĿ���ļ�
	file_generation(string, &head);

	//�ͷ��ڴ�
	freeRow(&head);
	
	return 1;
}