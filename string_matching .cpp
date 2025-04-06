#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data_structure.h"
#include "Character_storage.h"
#pragma warning(disable:4996) 

bool keyword_mach(char* target,char* key_word) {
	return (strcmp(target, key_word) == 0 );
}

bool both_key_mach(char** target, char* key,int num) {
	int i = 0;
	while (i<num) {
		if(strcmp(target[i], key) == 0)	return 1==1;
		i++;
	}
	return 1 == 2;
}

void keyword_ergodic(Row_data* head,char** target,int count,int* num) {
	//����������
	Row_data* node = head->next;
	while (node) {
		//�����ؼ�������
		int flag = 0;
		keyword* key_node = node->Keyword_linked_list;
		while (key_node) {
			if (keyword_mach(target[count], key_node->variable)) {
				node->flag = 1;
				flag = 1;
				key_node = key_node->next;
				break;
			}
			key_node = key_node->next;
		}
		//�������������
		opera* num_math = node->Operator_chain_table;
		while (num_math && flag == 1) {
			//����"="�������⴦��
			if (num_math->numeric_opera == '=' ) {
				//�Ƿ��ڵȺ���ߣ����ǾͲ������
				keyword* tmp = node->Keyword_linked_list;
				while (tmp->next)	tmp = tmp->next;
				if (!both_key_mach(target, tmp->variable,*num)) {
					node->flag = 0;
					break;
				}

				//���λ�ô���BUG
				else {
					//�ڵȺ���ߣ��Ͱ��ұߵı�����ŵ�Ŀ��������
					keyword* saving_target = node->Keyword_linked_list;
					while (!keyword_mach(saving_target->variable, target[count])) {
						if (saving_target->variable[0] >= '0' && saving_target->variable[0] <= '9') {
							saving_target = saving_target->next;
							continue;
						}
						record_target_words(target, saving_target->variable, (*num)++);
						saving_target = saving_target->next;
					}
					break;
				}
			}
			num_math = num_math->next;
		}
		node = node->next;
	}
}