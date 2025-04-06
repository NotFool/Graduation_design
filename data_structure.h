#pragma once
//�洢�ؼ�������Ϣ
typedef struct keyword {
	char variable[1000];
	struct keyword* next;
}; keyword;

//�洢�����������Ϣ
typedef struct opera {
	char numeric_opera;
	struct opera* next;
}; opera;

//�洢�����ݣ�ȥ���޹عؼ���
typedef struct Row_data {//������
	int flag;//�洢ƥ����Ϣ��ƥ��ɹ�Ϊ1
	int row_num;//�к�
	keyword* Keyword_linked_list;//�йؼ�������
	opera* Operator_chain_table;//�����������
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
