#include "string.h"

void string_init(){
	static int flag_complete_init = 0; // 이미 init을 호출한 적이 있는지 체크
	char file_path[250] = {0}; // 파일의 경로가 들어갈 배열
	char strbuf[MAX_STRING_LENGTH + 1] = {0}; // 파일로부터 단어를 읽어올 버퍼
	char *ptr = NULL; // 임시 포인터 변수
	FILE *fp = 0;

	if (flag_file_to_array != 0)
		return;

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

	flag_complete_init = 1;
}
char* get_word(int min, int max){
	int length = rand() % (max - min + 1) + min;
	return string[length][rand() % string_count[length]];
}

