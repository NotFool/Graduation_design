#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data_structure.h"
#include "file_processing.h"
#include "Character_storage.h"
#include "string_matching.h"
#include "Result_generation.h"
#pragma warning(disable:4996) 


//测试文件读取功能代码
int main() {
	//获取文件位置
	char string[100];
	scanf("%s", &string);
	int flag;
	
	//处理文件，获得链表结构
	Row_data* head;
	head = NULL;
	flag = file_get(string,&head);
	if (!flag) {
		printf("请输入正确的文件地址！\n");
		return 0;
	}
	
	//获取需要进行匹配的字符串
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

	//将待对比字符跟关键字链表进行对比，匹配成功修改flag
	int count = 0;
	while (count < num) {
		keyword_ergodic(head,Target_words,count++,&num);
	}

	free_words(Target_words, num);
	
	//打印链表结构
	//printRow(head);
	
	// 打印目标文件
	file_generation(string, &head);

	//释放内存
	freeRow(&head);
	
	return 1;
}