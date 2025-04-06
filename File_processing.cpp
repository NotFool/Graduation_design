#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "file_processing.h"
#include "data_structure.h"
#pragma warning(disable:4996) 

//判断是不是算数运算符
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

//按行处理文件内容，提取出其中的关键字，返回值为当前行字符串个数
int data_get(char* line,char strings[MAX_TOKENS][MAX_TOKEN_LENGTH]) {
	char* tokens[1000];
	char deliniters[] =" (), \n";
	int count = 0;

	//以空格为单位获取字符串
	char* token = strtok(line, deliniters);
	while (token != NULL) {
		tokens[count++] = token;
		strncpy(strings[count - 1], tokens[count - 1], MAX_TOKEN_LENGTH );
		token = strtok(NULL, deliniters);
	}

	return count;
}

//按行打开文件
int file_get(char* filename, Row_data** head) {
	//打开文件，打开错误返回值为0
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("打开文件错误！\n");
		return 0;
	}

	//行数据链表初始化位置

	Row_data* prev = NULL;

	*head = InitRowdata();
	prev = *head;
	//以行为单位读取文件
	char line[1000];
	int Row_num = 0;
	while (fgets(line, sizeof(line), file) != NULL) {
		Row_num++;
		//printf("这是第%d行，内容为：%s\n", Row_num, line);

		//此处应该创建行数据链表
		insertRowdata(&prev, Row_num);

		//进行数据提取
		int count;
		char strings[MAX_TOKENS][MAX_TOKEN_LENGTH];
		count = data_get(line,strings);

		//数据分类并进行存储
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

	//关闭文件，正常结束返回值为1
	fclose(file);
	return 1;
}