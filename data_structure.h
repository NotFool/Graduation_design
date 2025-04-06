#pragma once
//存储关键变量信息
typedef struct keyword {
	char variable[1000];
	struct keyword* next;
}; keyword;

//存储数字运算符信息
typedef struct opera {
	char numeric_opera;
	struct opera* next;
}; opera;

//存储行数据，去除无关关键字
typedef struct Row_data {//行数据
	int flag;//存储匹配信息，匹配成功为1
	int row_num;//行号
	keyword* Keyword_linked_list;//行关键字链表
	opera* Operator_chain_table;//行运算符链表
	struct Row_data* prev;
	struct Row_data* next;
}; Row_data;


Row_data* InitRowdata();
Row_data* creatRowdata(int Row);
keyword* creatKeyword(char* string);
opera* creatOpera(char num_opera);
void insertRowdata(Row_data** prev, int Row);
void insertKeyword(Row_data* Row, char* string);
void insertOpera(Row_data* Row, char oper);
void printKeyword(keyword* key_word);
void printOper(opera* num_opera);
void printRow(Row_data* head);
void freeKeyword(keyword* key);
void freeOpera(opera* num_oper);
void freeRow(Row_data** Row);
