#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "file_processing.h"
#include "data_structure.h"
#include "cJSON.h"
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
	char deliniters[] =" (), \ {}[]!#&*+-> \n";
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

//比较是否存在重复关键变量
int cmpstr(const void* a, const void* b) {
	return strcmp(*(char**)a, *(char**)b);
}

/* strndup自实现 */
static size_t my_strnlen(const char* s, size_t maxlen) {
	size_t i;
	for (i = 0; i < maxlen && s[i]; ++i);
	return i;
}

static char* my_strndup(const char* s, size_t n) {
	size_t len = my_strnlen(s, n);
	char* dup = (char*)malloc(len + 1);
	if (!dup) return NULL;
	memcpy(dup, s, len);
	dup[len] = '\0';
	return dup;
}

void JSON_pro(FILE* file,char** keyword,int* len) {
	//读取整个文件放入内存以方便进一步处理
	long size = ftell(file);
	rewind(file);

	char* buff = (char*)malloc(size + 1);
	if (!buff) {
		printf("读取JSON文件进入缓冲区失败！\n");
		free(buff);
		return;
	}

	fread(buff, 1, size, file);
	buff[size] = '\0';

	//解析JSON文件
	cJSON* root = cJSON_Parse(buff);
	if (!root) {
		printf("JSON parse error before: %s\n", cJSON_GetErrorPtr());
		free(buff);
	}
	free(buff);

	//动态数组初始化
	size_t cap = 16;

	//提取关键变量
	cJSON* item;
	if (cJSON_IsArray(root)) {
		cJSON_ArrayForEach(item, root) {
			cJSON* kv = cJSON_GetObjectItemCaseSensitive(item, "key_variable");
			if (cJSON_IsString(kv) && kv->valuestring) {
				char* s = kv->valuestring;
				char* arrow = strstr(s, "->");
				if (arrow) {
					/* 拆分 "name->word" 为两部分 */
					size_t name_len = arrow - s;
					char* name = my_strndup(s, name_len);
					char* word = strdup(arrow + 2);
					/* 确保容量 */
					for (int k = 0; k < 2; ++k) {
						if (*len + 1 > cap) {
							cap *= 2;
							char** tmp = (char**)realloc(keyword, cap * sizeof * keyword);
							if (!tmp) {
								printf("Memory allocation failed\n");
								/* 释放已分配内存 */
								free(name);
								free(word);
								cJSON_Delete(root);
								for (size_t i = 0; i < *len; ++i) free(keyword[i]);
								free(keyword);
								return;
							}
							keyword = tmp;
						}
						keyword[(*len)++] = (k == 0 ? name : word);
					}
				}
				else {
					/* 普通字符串，直接复制 */
					if (*len + 1 > cap) {
						cap *= 2;
						char** tmp = (char**)realloc(keyword, cap * sizeof * keyword);
						if (!tmp) {
							printf("Memory allocation failed\n");
							cJSON_Delete(root);
							for (size_t i = 0; i < *len; ++i) free(keyword[i]);
							free(keyword);
							return ;
						}
						keyword = tmp;
					}
					keyword[(*len)++] = strdup(s);
				}
			}
		}
	}
	else if (cJSON_IsObject(root)) {
		cJSON* kv = cJSON_GetObjectItemCaseSensitive(root, "key_variable");
		if (cJSON_IsString(kv) && kv->valuestring) {
			char* s = kv->valuestring;
			char* arrow = strstr(s, "->");
			if (arrow) {
				size_t name_len = arrow - s;
				keyword[(*len)++] = my_strndup(s, name_len);
				keyword[(*len)++] = strdup(arrow + 2);
			}
			else {
				keyword[(*len)++] = strdup(s);
			}
		}
	}
	cJSON_Delete(root);

	//去重
	size_t write_idx = 0;
	for (size_t read_idx = 0; read_idx < *len; ++read_idx) {
		int is_dup = 0;
		/* 检查是否在 keyword[0..write_idx-1] 中出现过 */
		for (size_t j = 0; j < write_idx; ++j) {
			if (strcmp(keyword[read_idx], keyword[j]) == 0) {
				is_dup = 1;
				break;
			}
		}
		if (!is_dup) {
			/* 将新元素移动到 write_idx 位置 */
			keyword[write_idx++] = keyword[read_idx];
		}
		else {
			/* 释放重复字符串内存 */
			free(keyword[read_idx]);
		}
	}
	*len = write_idx;
}