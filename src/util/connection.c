#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

#define PORTNUM_SCORE 8281
#define PORTNUM_MULTI 8182
#define SERVERADDRESS "155.230.52.58"

int socket_id_score = 0;
FILE* fp_score = NULL;

void send_score(int score, char *name);
int receive_score(int score[], char name[][4]);
void open_score_server();
void close_score_server();

// test용
int main(){
	int len = 0;
	int score[10] = {0};
	char name[10][4] = {""};
	
	open_score_server();
	send_score(10, "ASD");
	len = receive_score(score, name);
	close_score_server();

	for(int i = 0; i < len; i++)
		printf("%s %d\n", name[i], score[i]);

	return 0;
}

void open_score_server(){
	struct sockaddr_in servadd; // the number to call
	struct hostent *hp; // used to get number
	char message[BUFSIZ]; // to send message
	int messlen; // for message length 

	socket_id_score = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_id_score == -1){
		printf("open_score_server socket open error\n");
		exit(1);
	}

	bzero(&servadd, sizeof(servadd));
	hp = gethostbyname(SERVERADDRESS);
	if (hp == NULL){
		printf("gethostbtname error");
		exit(1);
	}
	bcopy(hp->h_addr, (struct sockaddr *)&servadd.sin_addr, hp->h_length);
	servadd.sin_port = htons(PORTNUM_SCORE);
	servadd.sin_family = AF_INET;

	if (connect(socket_id_score, (struct sockaddr *)&servadd, sizeof(servadd)) != 0){
		printf("open_score_server connect 문제 발생");
		exit(1);
	}

	fp_score = fdopen(socket_id_score, "w+");
}
// score server를 닫아줌
void close_score_server(){
	fclose(fp_score);
	socket_id_score = -1;
}

// 점수를 서버로 보냄. int score : 점수(초단위), char *name : 이름
void send_score(int score, char *name){
	if (socket_id_score == -1)
		open_score_server();

	printf("send_score 시작\n");
	fprintf(fp_score, "%s %d\n", name, score);
	printf("send_score 끝\n");
}

// 서버로부터 점수를 받아옴. 상위 10등까지의 점수를 받아온다.
// score 배열과 name 배열에 각각 점수와 이름이 들어감.
// index : 0부터 시작 
int receive_score(int score[], char name[][4]){
	if (socket_id_score == -1)
		open_score_server();

	int len = 0;

	fscanf(fp_score, "%d", &len);
	for(int i = 0; i < len; i++)
		fscanf(fp_score, "%s %d", name[i], &score[i]);
	fclose(fp_score);

	return len;
}
