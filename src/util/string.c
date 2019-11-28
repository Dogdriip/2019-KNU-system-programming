#include "string.h"

char string[MAX_STRING_LENGTH + 1][MAX_STRING_COUNT][MAX_STRING_LENGTH+1];
int string_count[MAX_STRING_LENGTH + 1]; // 각 길이당 몇개의 문장을 가지고 있는지 저장하는 배열
	
int get_string_count(int i){
	return string_count[i];
}

void string_init(){
	static int flag_complete_init = 0; // 이미 init을 호출한 적이 있는지 체크
	char strbuf[MAX_STRING_LENGTH + 2] = ""; // 단어를 읽어올 버퍼
	FILE *fp = 0;

	if (flag_complete_init != 0)
		return;
	
	struct sockaddr_in servadd;
	struct hostent *hp;
	int sock_id = socket(AF_INET, SOCK_STREAM, 0);
	
	bzero(&servadd, sizeof(servadd));
	hp = gethostbyname(SERVERADDRESS);
	bcopy(hp->h_addr, (struct sockaddr *)&servadd.sin_addr, hp->h_length);
	servadd.sin_port = htons(PORTNUM_STRING);
	servadd.sin_family = AF_INET;

	connect(sock_id, (struct sockaddr *)&servadd, sizeof(servadd));
	
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
	flag_complete_init = 1;
}
char* get_word(int min, int max){
	int length = rand() % (max - min + 1) + min;
	return string[length][rand() % string_count[length]];
}

