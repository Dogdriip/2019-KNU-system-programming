#include "connection.h"

int socket_id_multi = 0;

void* multi_connection(void* m){
	struct sockaddr_in servadd;
	struct hostent *hp;
	char message[BUFSIZ]="", *msg_loading = "loading", *msg_match = "match11";
	int row, col;
	multi_info *info = (multi_info*)m; // 연결 제대로 되면 1, 안되면 -1

	info->flag = 0;

	socket_id_multi = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_id_multi == -1){
		info->flag = -1;
		return NULL;
	}

	bzero(&servadd, sizeof(servadd));
	hp = gethostbyname(SERVERADDRESS);
	if (hp == NULL){
		info->flag = -1;
		return NULL;
	}
	bcopy(hp->h_addr, (struct sockaddr *)&servadd.sin_addr, hp->h_length);
	servadd.sin_port = htons(PORTNUM_MULTI);
	servadd.sin_family = AF_INET;

	if (connect(socket_id_multi, (struct sockaddr *)&servadd, sizeof(servadd)) != 0){
		info->flag = -1;
		return NULL;
	}
	
	info->fd = socket_id_multi;
	
	// 연결 성공하면, loading or match.
	// loading은 다른 클라이언트를 기다리는 상태.
	// match는 잡힌 상태
	//read(info->fd, info->message, (strlen(msg_loading) + 1)* sizeof(char));
	read(info->fd, info->message, (20)* sizeof(char));
	if (strcmp(info->message, "loading") == 0){
		//read(info->fd, info->message, (strlen(msg_match) + 1) * sizeof(char));
		read(info->fd, info->message, (20) * sizeof(char));
		int temp = 1;
		write(info->fd, &temp, sizeof(int));
	}

	info->flag = 1;

	return NULL;
}

int open_score_server(){
	struct sockaddr_in servadd; // the number to call
	struct hostent *hp; // used to get number
	char message[BUFSIZ]; // to send message
	int messlen; // for message length 
	int socket_id_score;

	socket_id_score = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_id_score == -1){
		printf("open_score_server socket open error\n");
		return -1;
	}

	bzero(&servadd, sizeof(servadd));
	hp = gethostbyname(SERVERADDRESS);
	if (hp == NULL){
		printf("gethostbyname error");
		return -1;
	}
	bcopy(hp->h_addr, (struct sockaddr *)&servadd.sin_addr, hp->h_length);
	servadd.sin_port = htons(PORTNUM_SCORE);
	servadd.sin_family = AF_INET;

	if (connect(socket_id_score, (struct sockaddr *)&servadd, sizeof(servadd)) != 0){
		printf("open_score_server connect 문제 발생");
		return -1;
	}

	return socket_id_score;
}
// score server를 닫아줌
void close_score_server(int socket_id){
	close(socket_id);
}

// 점수를 서버로 보냄.int mode : 0일때  int score : 점수(초단위), char *name : 이름
int send_score(int socket_id, int mode, int score, char *name){
	if (socket_id == -1)
		return -1;

	write(socket_id, &mode, sizeof(int));
	write(socket_id, name, sizeof(char) * 4);
	write(socket_id, &score, sizeof(int));

	return 0;
}

// 서버로부터 점수를 받아옴. 상위 10등까지의 점수를 받아온다.
// score 배열과 name 배열에 각각 점수와 이름이 들어감.
// index : 0부터 시작 
int receive_score(int socket_id, int mode, int score[], char name[][4]){
	if (socket_id == -1)
		return -1;

	int len = 0;

	read(socket_id, &len, sizeof(int));
	for(int i = 0; i < len; i++){
		read(socket_id, name[i], sizeof(char) * 4);
		read(socket_id, &score[i], sizeof(int));
	}

	return len;
}
