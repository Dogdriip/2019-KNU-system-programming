#include "string.h"

char string[MAX_STRING_LENGTH + 1][MAX_STRING_COUNT][MAX_STRING_LENGTH+1];
int string_count[MAX_STRING_LENGTH + 1]; // 각 길이당 몇개의 문장을 가지고 있는지 저장하는 배열
	
int get_string_count(int i){
	return string_count[i];
}

// mode == 0 : 서버에서, mode == 1 : 로컬에서
int string_init(int mode){
	static int flag_complete_init = 0; // 이미 init을 호출한 적이 있는지 체크
	char strbuf[MAX_STRING_LENGTH + 2] = ""; // 단어를 읽어올 버퍼
	FILE *fp = 0;

	if (flag_complete_init != 0)
		return 1;

	if (mode == 0){	
		struct sockaddr_in servadd;
		struct hostent *hp;
		int sock_id = socket(AF_INET, SOCK_STREAM, 0);
		
		bzero(&servadd, sizeof(servadd));
		hp = gethostbyname(SERVERADDRESS);
		bcopy(hp->h_addr, (struct sockaddr *)&servadd.sin_addr, hp->h_length);
		servadd.sin_port = htons(PORTNUM_STRING);
		servadd.sin_family = AF_INET;

		if(connect(sock_id, (struct sockaddr *)&servadd, sizeof(servadd)) == -1){
			return -1;
		}
	
		fp = fdopen(sock_id, "r");

		fscanf(fp, "%s", strbuf);
		while(!feof(fp)){
			int len = strlen(strbuf);

			if (string_count[len] < MAX_STRING_COUNT){
			strcpy(string[len][string_count[len]], strbuf);
				string_count[strlen(strbuf)]++;
			}
			fscanf(fp, "%s", strbuf);
		}

		fclose(fp);
	}
	else{
		char file_path[250] = ""; // 파일의 경로가 들어갈 배열
		char *ptr = NULL; // 임시 포인터 변수

		getcwd(file_path, 250);

		ptr = strstr(file_path, "2019-KNU-system-programming");
		if (ptr == NULL){
			fprintf(stderr, "게임 파일을 게임 폴더 내부에서실행시켜 주세요.\n");
			exit(1);
		}
		*(ptr + strlen("2019-KNU-system-programming")) = '\0';

		strcat(file_path, "/resource/word.txt");
		
		fp = fopen(file_path, "r");

		fscanf(fp, "%s", strbuf);
		while(!feof(fp)){
			int length = strlen(strbuf);

			if (MIN_STRING_LENGTH <= length && length <= MAX_STRING_LENGTH && string_count[length] < MAX_STRING_COUNT){
				strcpy(string[length][string_count[length]], strbuf);

				string_count[length]++;
			}
			fscanf(fp, "%s", strbuf);
		}
		fclose(fp);
	}
	flag_complete_init = 1;
	return 1;
}
char* get_word(int min, int max){
	int length = rand() % (max - min + 1) + min;
	return string[length][rand() % string_count[length]];
}

