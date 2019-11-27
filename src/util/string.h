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

void string_init(); // 파일에서 단어를 가져와서 string 버퍼에 저장.
char* get_word(int min, int max); // 단어를 얻음
int get_count(int i);
