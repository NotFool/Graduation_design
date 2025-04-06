#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data_structure.h"
#pragma warning(disable:4996) 

//�����ݳ�ʼ������������ͷ�ڵ�
Row_data* InitRowdata() {
	//����ͷ���ռ�
	Row_data* head = (Row_data*)malloc(sizeof(Row_data));
	//�ж��Ƿ����ռ�ɹ�
	if (!head) {
		printf("�ڴ����ʧ�ܣ�\n");
		return NULL;
	}

	//ͷ����ʼ��
	head->flag = 0;
	head->row_num = 0;
	head->Keyword_linked_list = NULL;
	head->Operator_chain_table = NULL;
	head->prev = NULL;
	head->next = NULL;

	return head;
}

//����������ڵ�
Row_data* creatRowdata(int Row){
	//�����ڴ�ռ�
	Row_data* new_Row = (Row_data*)malloc(sizeof(Row_data));
	if (!new_Row) {
		printf("�ڴ����ʧ�ܣ�\n");
		return NULL;
	}

	//���ݵ���
	new_Row->flag = 0;
	new_Row->row_num = Row;
	new_Row->Keyword_linked_list = NULL;
	new_Row->Operator_chain_table = NULL;
	new_Row->prev = NULL;
	new_Row->next = NULL;

	return new_Row;
}

//�����ؼ�������ڵ�
keyword* creatKeyword(char* string) {
	//�����ڴ�ռ�
	keyword* new_key = (keyword*)malloc(sizeof(keyword));
	if (!new_key) {
		printf("�ڴ����ʧ�ܣ�\n");
		return NULL;
	}

	//���ݵ���
	strcpy(new_key->variable, string);
	new_key->next = NULL;
	
	return new_key;
}

//������ѧ������ڵ�
opera* creatOpera(char num_opera) {
	//�����ڴ�ռ�
	opera* new_oper = (opera*)malloc(sizeof(opera));
	if (!new_oper) {
		printf("�ڴ����ʧ�ܣ�\n");
		return NULL;
	}

	//���ݵ���
	new_oper->numeric_opera = num_opera;
	new_oper->next = NULL;

	return new_oper;
}

//����������
void insertRowdata(Row_data** prev, int Row) {
	Row_data* new_Row = creatRowdata(Row);
	if (!new_Row)	return;

	//������
	(*prev)->next = new_Row;
	new_Row->prev = *prev;
	
	//ǰ������ˢ��
	*prev = new_Row;
}

//����ؼ�������
void insertKeyword(Row_data* Row, char* string) {
	if (!Row)	return;

	//�ڵ�����
	keyword* new_key = creatKeyword(string);
	if (!new_key)	return;

	//������
	new_key->next = Row->Keyword_linked_list;
	Row->Keyword_linked_list = new_key;
}

//�������������
void insertOpera(Row_data* Row, char oper) {
	if (!Row)	return;

	//�ڵ�����
	opera* new_opra = creatOpera(oper);
	if (!new_opra)	return;

	//������
	new_opra->next = Row->Operator_chain_table;
	Row->Operator_chain_table = new_opra;
}

//�����ؼ�������
void printKeyword(keyword* key_word) {
	printf("[");
	while (key_word) {
		printf("%s ", key_word->variable);
		key_word = key_word->next;
	}
	printf("]");
}

//��������������
void printOper(opera* num_opera) {
	printf("[");
	while (num_opera) {
		printf("%c ", num_opera->numeric_opera);
		num_opera = num_opera->next;
	}
	printf("]");
}

//�����ݱ���
void printRow(Row_data* head) {
	while (head) {
		if (head->flag == 1) {
			printf("�кţ�%d\n", head->row_num);
			printf("�ؼ��ְ�����");
			printKeyword(head->Keyword_linked_list);
			printf("\n");
			printf("���������������");
			printOper(head->Operator_chain_table);
			printf("\n");
		}
		head = head->next;
	}
}

//�ͷŹؼ�������
void freeKeyword(keyword* key) {
	while (key) {
		keyword* tmp = key;
		key = key->next;
		free(tmp);
	}
}

//�ͷ��������������
void freeOpera(opera* num_oper) {
	while (num_oper) {
		opera* tmp = num_oper;
		num_oper = num_oper->next;
		free(tmp);
	}
}

//�ͷ�����������
void freeRow(Row_data** Row) {
	while (*Row) {
		Row_data* tmp = *Row;
		freeKeyword(tmp->Keyword_linked_list);
		freeOpera(tmp->Operator_chain_table);
		*Row = (*Row)->next;
		free(tmp);
	}
}