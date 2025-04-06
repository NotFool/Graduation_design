#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data_structure.h"
#pragma warning(disable:4996) 

//行数据初始化函数，构建头节点
Row_data* InitRowdata() {
	//分配头结点空间
	Row_data* head = (Row_data*)malloc(sizeof(Row_data));
	//判断是否分配空间成功
	if (!head) {
		printf("内存分配失败！\n");
		return NULL;
	}

	//头结点初始化
	head->flag = 0;
	head->row_num = 0;
	head->Keyword_linked_list = NULL;
	head->Operator_chain_table = NULL;
	head->prev = NULL;
	head->next = NULL;

	return head;
}

//创建行链表节点
Row_data* creatRowdata(int Row){
	//分配内存空间
	Row_data* new_Row = (Row_data*)malloc(sizeof(Row_data));
	if (!new_Row) {
		printf("内存分配失败！\n");
		return NULL;
	}

	//数据导入
	new_Row->flag = 0;
	new_Row->row_num = Row;
	new_Row->Keyword_linked_list = NULL;
	new_Row->Operator_chain_table = NULL;
	new_Row->prev = NULL;
	new_Row->next = NULL;

	return new_Row;
}

//创建关键字链表节点
keyword* creatKeyword(char* string) {
	//分配内存空间
	keyword* new_key = (keyword*)malloc(sizeof(keyword));
	if (!new_key) {
		printf("内存分配失败！\n");
		return NULL;
	}

	//数据导入
	strcpy(new_key->variable, string);
	new_key->next = NULL;
	
	return new_key;
}

//创建数学运算符节点
opera* creatOpera(char num_opera) {
	//分配内存空间
	opera* new_oper = (opera*)malloc(sizeof(opera));
	if (!new_oper) {
		printf("内存分配失败！\n");
		return NULL;
	}

	//数据导入
	new_oper->numeric_opera = num_opera;
	new_oper->next = NULL;

	return new_oper;
}

//插入行数据
void insertRowdata(Row_data** prev, int Row) {
	Row_data* new_Row = creatRowdata(Row);
	if (!new_Row)	return;

	//链表构成
	(*prev)->next = new_Row;
	new_Row->prev = *prev;
	
	//前置链表刷新
	*prev = new_Row;
}

//插入关键字数据
void insertKeyword(Row_data* Row, char* string) {
	if (!Row)	return;

	//节点生成
	keyword* new_key = creatKeyword(string);
	if (!new_key)	return;

	//链表构建
	new_key->next = Row->Keyword_linked_list;
	Row->Keyword_linked_list = new_key;
}

//插入数字运算符
void insertOpera(Row_data* Row, char oper) {
	if (!Row)	return;

	//节点生成
	opera* new_opra = creatOpera(oper);
	if (!new_opra)	return;

	//链表构建
	new_opra->next = Row->Operator_chain_table;
	Row->Operator_chain_table = new_opra;
}

//遍历关键字链表
void printKeyword(keyword* key_word) {
	printf("[");
	while (key_word) {
		printf("%s ", key_word->variable);
		key_word = key_word->next;
	}
	printf("]");
}

//遍历算数符链表
void printOper(opera* num_opera) {
	printf("[");
	while (num_opera) {
		printf("%c ", num_opera->numeric_opera);
		num_opera = num_opera->next;
	}
	printf("]");
}

//行数据遍历
void printRow(Row_data* head) {
	while (head) {
		if (head->flag == 1) {
			printf("行号：%d\n", head->row_num);
			printf("关键字包含：");
			printKeyword(head->Keyword_linked_list);
			printf("\n");
			printf("算数运算符包含：");
			printOper(head->Operator_chain_table);
			printf("\n");
		}
		head = head->next;
	}
}

//释放关键字链表
void freeKeyword(keyword* key) {
	while (key) {
		keyword* tmp = key;
		key = key->next;
		free(tmp);
	}
}

//释放算术运算符链表
void freeOpera(opera* num_oper) {
	while (num_oper) {
		opera* tmp = num_oper;
		num_oper = num_oper->next;
		free(tmp);
	}
}

//释放行数据链表
void freeRow(Row_data** Row) {
	while (*Row) {
		Row_data* tmp = *Row;
		freeKeyword(tmp->Keyword_linked_list);
		freeOpera(tmp->Operator_chain_table);
		*Row = (*Row)->next;
		free(tmp);
	}
}