#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

// 최소 길이 2, 최대 길이 12. 각 길이당 최대 1000개의 문장이 들어감
#define MAX_STRING_COUNT 1000
#define MIN_STRING_LENGTH 2
#define MAX_STRING_LENGTH 12

char* string[MAX_STRING_LENGTH + 1][MAX_STRING_COUNT];
int string_count[MAX_STRING_LENGTH + 1]; // 각 길이당 몇개의 문장을 가지고 있는지 저장하는 배열

void string_init(); // 파일에서 단어를 가져와서 string 버퍼에 저장.
char* get_word(int min, int max); // 단어를 얻음
