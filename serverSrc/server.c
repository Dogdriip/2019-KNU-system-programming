#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>

#define PORTNUM_STRING 8181
#define PORTNUM_SCORE 8281
#define PORTNUM_MULTI 8182
#define HOSTLEN 256
#define oops(msg) {perror(msg); exit(1);}

#define MIN_STRING_LENGTH 2
#define MAX_STRING_LENGTH 12
#define MAX_STRING_COUNT 1000

struct Score{
	int score;
	char name[4];
	int mode;
};
typedef struct multi_match{
	int fd1, fd2;
}multi_match;

char string[MAX_STRING_LENGTH + 1][MAX_STRING_COUNT][MAX_STRING_LENGTH + 1];
int string_count[MAX_STRING_LENGTH + 1];
int sock_multi_id;

pthread_t t1, t2, t3;

void* string_server(void*);

void* multi_server(void*);
void* multi_screen_communication(void*);
void* wait_other(void*);

void save_score(struct Score score);
int receive_score(int);
void send_score(int, int);
void* score_server(void*);

void signal_handler();

int main(){
	printf("서버 시작\n");

	pthread_create(&t1, NULL, score_server, NULL); 
	pthread_create(&t2, NULL, multi_server, NULL);
	pthread_create(&t3, NULL, string_server, NULL);

	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);

	printf("서버 종료\n");
}

void signal_handler(){
	pthread_cancel(t1);
	pthread_cancel(t2);
	pthread_cancel(t3);
	printf("스레드 종료 완료\n");
}

void* string_server(void* thread_data){
	char strbuf[20];

	FILE *fp = fopen("resource/word.txt", "r");

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

	printf("string 준비 완료\n");

	struct sockaddr_in saddr;
	struct hostent *hp;
	char hostname[HOSTLEN];
	int sock_id, sock_fd;

	sock_id = socket(PF_INET, SOCK_STREAM, 0);
	bzero((void *)&saddr, sizeof(saddr));

	gethostname(hostname, HOSTLEN);
	hp = gethostbyname(hostname);

	bcopy((void *)hp->h_addr, (void *)&saddr.sin_addr, hp->h_length);
	saddr.sin_port = htons(PORTNUM_STRING);
	saddr.sin_family = AF_INET;

	bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr));

	int optval = 1;
	listen(sock_id, 50);
	setsockopt(sock_id, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	printf("string 서버 대기 시작\n");
	while(1){
		sock_fd = accept(sock_id, NULL, NULL);
		printf("string 서버 연결 성공\n");

		fp = fdopen(sock_fd, "w");
		
		for(int i = MIN_STRING_LENGTH; i <= MAX_STRING_LENGTH; i++)
			for(int j = 0; j < string_count[i]; j++)
				fprintf(fp, "%s\n", string[i][j]);
		fclose(fp);
	}	
}

void* multi_server(void* thread_data){
	struct sockaddr_in saddr;
	struct hostent *hp;
	pthread_attr_t attr;
	char hostname[HOSTLEN];
	int sock_fd1, sock_fd2;

	sock_multi_id = socket(PF_INET, SOCK_STREAM, 0);
	if (sock_multi_id == -1)
		oops("mulit server socket");

	bzero((void *)&saddr, sizeof(saddr));

	gethostname(hostname, HOSTLEN);
	hp = gethostbyname(hostname);

	bcopy((void *)hp->h_addr, (void *)&saddr.sin_addr, hp->h_length);
	saddr.sin_port = htons(PORTNUM_MULTI);
	saddr.sin_family = AF_INET;

	if (bind(sock_multi_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
		oops("multi bind");

	if (listen(sock_multi_id, 200) != 0)
		oops("multi listen");

	// 쓰레드 detach 옵션 설정
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	while(1){
		struct multi_match *thr_data = (struct multi_match*)malloc(sizeof(struct multi_match));
		char message[30];
		printf("client1 accpt 대기\n");

		// 첫번째 클라이언트와 연결.
		thr_data->fd1 = accept(sock_multi_id, NULL, NULL);
		if (thr_data->fd1 == -1)
			oops("multi accept error");
		printf("client1 accept 완료\n");
		strcpy(message, "loading");
		write(thr_data->fd1, message, (strlen(message) + 1) * sizeof(char));
	
		printf("두번째 클라이언트 연결 대기\n");
		// 두번째 클라이언트와 연결
		pthread_t thread_wait;
		thr_data->fd2 = -1;

		pthread_create(&thread_wait, &attr, wait_other, (void*)thr_data);
		
		int flag = 0;
		read(thr_data->fd1, &flag, sizeof(int));
		printf("받은 값 : %d\n", flag);

		if (flag == -1){
			pthread_cancel(thread_wait);
			printf("첫번째 클라이언트 연결 취소\n");
			close(thr_data->fd1);
			continue;
		}

		printf("ok!\n");

		pthread_t thread_screen;
		pthread_create(&thread_screen, &attr, multi_screen_communication, (void*)thr_data);
	}
	pthread_attr_destroy(&attr);

}

// 두번째 클라이언트 연결
void* wait_other(void *m){
	multi_match *data = (multi_match*)m;	
	int fd2;
	char message[30] = "match11";
	sleep(1);

	fd2 = accept(sock_multi_id, NULL, NULL);
	if (fd2 == -1)
		oops("client2 accept error");
	data->fd2 = fd2;
	printf("client2 accept 완료\n");

	write(data->fd1, message, (strlen(message)+1) * sizeof(char));
	write(data->fd2, message, (strlen(message)+1) * sizeof(char));
	printf("match message 송신 완료\n");
}

// 스크린을 주고받는 함수
void* multi_screen_communication(void* thr_data){
	multi_match *data = (multi_match*)thr_data;
	int fd1 = data->fd1, fd2 = data->fd2;
	char message1[500000], message2[500000];
	int i = 0;
	
	int bufsize = 131072;
	setsockopt(fd1, SOL_SOCKET, SO_SNDBUF, &bufsize, sizeof(bufsize));
	setsockopt(fd1, SOL_SOCKET, SO_RCVBUF, &bufsize, sizeof(bufsize));
	setsockopt(fd2, SOL_SOCKET, SO_SNDBUF, &bufsize, sizeof(bufsize));
	setsockopt(fd2, SOL_SOCKET, SO_RCVBUF, &bufsize, sizeof(bufsize));
	
	while(1){
		int total_len1 = 0, total_len2 = 0, len1 = 0, len2 = 0;
		int flag1 = 0, flag2 = 0; // 게임이 끝났는지 체크

		read(fd1, &flag1, sizeof(int));
		read(fd2, &flag2, sizeof(int));
		if (flag1 == -1 || flag2 == -1){
			printf("게임 하나 끝남\n");
			if (flag1 == -1)
				write(fd2, &flag1, sizeof(int));
			if (flag2 == -1)
				write(fd1, &flag2, sizeof(int));
			close(fd1);
			close(fd2);
			return NULL;
		}
		write(fd1, &flag2, sizeof(int));
		write(fd2, &flag1, sizeof(int));

		read(fd1, &total_len1, sizeof(int));
		len1 = read(fd1, message1, sizeof(char) * total_len1);
		while(len1 < total_len1)
			len1 += read(fd1, &message1[len1], sizeof(char) * (total_len1 - len1));

		read(fd2, &total_len2, sizeof(int));
		len2 = read(fd2, message2, sizeof(char) * total_len2);
		while(len2 < total_len2) 
			len2 += read(fd2, &message2[len2], sizeof(char) * (total_len2 - len2));

		write(fd1, &total_len2, sizeof(int));
		write(fd1, message2, sizeof(char) * total_len2);
		write(fd2, &total_len1, sizeof(int));
		write(fd2, message1, sizeof(char) * total_len1);
	}
}
void* score_server(void* thread_data){
	struct sockaddr_in saddr;
	struct hostent *hp;
	char hostname[HOSTLEN];
	int sock_id, sock_fd;
	
	sock_id = socket(PF_INET, SOCK_STREAM, 0);
	if (sock_id == -1)
		oops("socket");

	bzero((void *) &saddr, sizeof(saddr));

	gethostname(hostname, HOSTLEN);
	hp = gethostbyname(hostname);

	bcopy((void *)hp->h_addr, (void *)&saddr.sin_addr, hp->h_length);
	saddr.sin_port = htons(PORTNUM_SCORE);
	saddr.sin_family = AF_INET;

	if (bind(sock_id, (struct sockaddr *) &saddr, sizeof(saddr)) != 0)
		oops("bind");

	if (listen(sock_id, 200) != 0)
		oops("listen");

	while(1){
		sock_fd = accept(sock_id, NULL, NULL);
		if (sock_fd == -1)
			oops("accept");

		printf("[score server] score receive start\n");
		int mode = receive_score(sock_fd);

		printf("[score server] score board send start\n");
		send_score(sock_fd, mode);
	
		close(sock_fd);
		printf("[score server] socket close\n");
	}
}
// sock_fd와 연결된 클라이언트에게 score_board를 보냄
void send_score(int sock_fd, int mode){
	FILE *fp = NULL;
	if (mode == 0)
		fp = fopen("resource/score.txt", "r");
	else
		fp = fopen("resource/score2.txt", "r");
	struct Score score;
	int len_score_board = 0;

	fscanf(fp, "%d", &len_score_board);
	write(sock_fd, &len_score_board, sizeof(int));
	for(int i = 0; i < len_score_board; i++){
		fscanf(fp, "%s %d", score.name, &(score.score));
		write(sock_fd, score.name, sizeof(char) * 4);
		write(sock_fd, &(score.score), sizeof(int));
		printf("%s %d, ", score.name, score.score);
	}

	fclose(fp);
}

// sock_fd와 연결된 클라이언트로부터 name과 score를 받아옴.
int receive_score(int sock_fd){
	struct Score score;

	read(sock_fd, &(score.mode), sizeof(int));
	read(sock_fd, score.name, sizeof(char) * 4);
	read(sock_fd, &(score.score), sizeof(int));
	printf("[%d] %s %d\n", score.mode, score.name,score.score);

	save_score(score);

	return score.mode;
}
// score를 파일에 저장함.
void save_score(struct Score score){
	struct Score score_board[11] = {{0,""}};
	int len_score_board = 0;
	int flag_update = 0;
	FILE *fp;
	
	if (score.mode == 0)
		fp = fopen("resource/score.txt", "r");
	else
		fp = fopen("resource/score2.txt", "r");

	// 파일로부터 점수를 읽어옴
	// 정상적이라면 len_score_board는 10개 이하이다.
	fscanf(fp, "%d", &len_score_board);
	for(int i = 0; i < len_score_board; i++)
		fscanf(fp, "%s %d", score_board[i].name, &(score_board[i].score));
	fclose(fp);

	// score_board를 재설정. 받아온 점수가 10등 이내면 업데이트 해줌.
	for(int i = 0; i < 10; i++){
		if (score_board[i].score < score.score){
			for(int j = 9; j > i; j--)
				score_board[j] = score_board[j-1];
			score_board[i] = score;
			flag_update = 1;
			len_score_board++;
			if (len_score_board > 10)
				len_score_board = 10;
			break;
		}
	}
	
	// score_board가 변경되었으면, 파일에도 업데이트 해줌
	if (flag_update == 1){
		if (score.mode == 0)
			fp = fopen("resource/score.txt", "w");
		else
			fp = fopen("resource/score2.txt", "w");
		fprintf(fp, "%d\n",len_score_board);
		for(int i = 0; i < len_score_board; i++)
			fprintf(fp, "%s %d\n",score_board[i].name, score_board[i].score);
		fclose(fp);
	}
}
