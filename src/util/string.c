#include "string.h"

char string[MAX_STRING_LENGTH + 1][MAX_STRING_COUNT][MAX_STRING_LENGTH+1];
int string_count[MAX_STRING_LENGTH + 1]; // 각 길이당 몇개의 문장을 가지고 있는지 저장하는 배열

int main(){
	string_init();

	for(int i = 2; i <= 12; i++)
		printf("%d\n", string_count[i]);

	printf("%s\n",get_word(2, 12));

	return 0;
}
	
int get_count(int i){
	return string_count[i];
}

void string_init(){
	static int flag_complete_init = 0; // 이미 init을 호출한 적이 있는지 체크
	char file_path[250] = {0}; // 파일의 경로가 들어갈 배열
	char strbuf[MAX_STRING_LENGTH + 2] = ""; // 파일로부터 단어를 읽어올 버퍼
	char *ptr = NULL; // 임시 포인터 변수
	FILE *fp = 0;

	if (flag_complete_init != 0)
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
	fscanf(fp, "%s", strbuf);
	while(!feof(fp)){
		int length = strlen(strbuf);
		printf("%s %d\n",strbuf, length);
		
		if (MIN_STRING_LENGTH <= length && length <= MAX_STRING_LENGTH && string_count[length] < MAX_STRING_COUNT){
			strcpy(string[length][string_count[length]], strbuf);

			string_count[length]++;
		}
		fscanf(fp, "%s", strbuf);
	}
	fclose(fp);

	flag_complete_init = 1;
}
char* get_word(int min, int max){
	int length = rand() % (max - min + 1) + min;
	return string[length][rand() % string_count[length]];
}

