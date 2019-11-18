#include "string.h"

void string_init(){
	// 파일에서 문장을 가져와서, string 버퍼에 저장함. srand()도 호출함.
	char file_path[250] = {0};
	char strbuf[MAX_STRING_LENGTH + 1] = {0};
	char *ptr = NULL;
	FILE *fp = 0;

	srand((long)time(NULL));

	// 리소스 폴더의 string.txt의 경로를 얻는 코드
	getcwd(file_path, 250);

	ptr = strstr(file_path, "2019-KNU-system-programming");
	if (ptr == NULL){
		fprintf(stderr, "게임 파일을 게임 폴더 내부에서 실행시켜 주세요.\n");
		exit(1);
	}
	*(ptr + strlen("2019-KNU-system-programming")) = '\0';
	
	strcat(file_path, "/resource/word.txt");

	fp = fopen(file_path, "r");	

	// 게임에서 사용할 문장들을 가져오는 코드
	fgets(strbuf, MAX_STRING_LENGTH + 1, fp);
	while(!feof(fp)){
		int length = strlen(strbuf);
		
		if (MIN_STRING_LENGTH < length && length < MAX_STRING_LENGTH \
				&& string_count[length] < MAX_STRING_COUNT){
			string[length][string_count[length]] = (char*)malloc(sizeof(char) * (length + 1));
			strcpy(string[length][string_count[length]], strbuf);

			string_count[length]++;
		}
		fgets(strbuf, MAX_STRING_LENGTH + 1, fp);
	}
	fclose(fp);
}
char* get_word(int min, int max){
	int length = rand() % (max - min + 1) + min;
	return string[length][rand() % string_count[length]];
}

