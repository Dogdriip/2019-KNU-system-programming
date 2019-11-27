#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define PORTNUM_SCORE 8281
#define PORTNUM_MULTI 8182
#define HOSTLEN 256
#define oops(msg) {perror(msg); exit(1);}

struct Score{
	int score;
	char name[4];
};
struct multi_match{
	FILE *fp1, *fp2;
};

FILE *sock_score_fp = NULL;

void* multi_server(void*);
void* multi_screen_communication(void*);

void save_score(struct Score score);
void receive_score();
void send_score();
void* score_server(void*);

int main(){
	pthread_t t1, t2;

	pthread_create(&t1, NULL, score_server, NULL); 
	pthread_create(&t2, NULL, multi_server, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
}

void* multi_server(void* thread_data){
	struct sockaddr_in saddr;
	struct hostent *hp;
	pthread_attr_t attr;
	char hostname[HOSTLEN];
	int sock_id, sock_fd1, sock_fd2;

	sock_id = socket(PF_INET, SOCK_STREAM, 0);
	if (sock_id == -1)
		oops("mulit server socket");

	bzero((void *)&saddr, sizeof(saddr));

	gethostname(hostname, HOSTLEN);
	hp = gethostbyname(hostname);

	bcopy((void *)hp->h_addr, (void *)&saddr.sin_addr, hp->h_length);
	saddr.sin_port = htons(PORTNUM_MULTI);
	saddr.sin_family = AF_INET;

	if (bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
		oops("multi bind");

	if (listen(sock_id, 200) != 0)
		oops("multi listen");

	// 쓰레드 detach 옵션 설정
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	while(1){
		struct multi_match *thr_data = (struct multi_match*)malloc(sizeof(struct multi_match));
		
		// 첫번째 클라이언트와 연결.
		sock_fd1 = accept(sock_id, NULL, NULL);
		if (sock_fd1 == -1)
			oops("multi accept error");
		printf("client1 accept 완료\n");
		thr_data->fp1 = fdopen(sock_fd1, "w+");
		if (thr_data->fp1 == NULL)
			oops("client1 fdopen failed");
		fprintf(thr_data->fp1, "loading");
		fflush(thr_data->fp1);
		
		// 두번째 클라이언트와 연결
		sock_fd2 = accept(sock_id, NULL, NULL);
		if (sock_fd2 == -1)
			oops("multi accept error2");
		printf("client2 accept 완료\n");
		thr_data->fp2 = fdopen(sock_fd2, "w+");
		if (thr_data->fp2 == NULL)
			oops("client2 fdopen failed");
		fprintf(thr_data->fp1, "match");
		fprintf(thr_data->fp2, "match");
		fflush(thr_data->fp1);
		fflush(thr_data->fp2);
	}
	pthread_attr_destroy(&attr);

}

void* multi_screen_communication(void* thr_data){
	struct multi_match *data = (struct multi_match*)thr_data;
	

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
		printf("accept 완료\n");
		sock_score_fp = fdopen(sock_fd, "w+");

		printf("receive 시작\n");
		receive_score();
		printf("receive 완료\n");

		printf("send 시작\n");
		send_score();
		printf("send 완료\n");
	
		fclose(sock_score_fp);
		printf("socket close\n");
	}
}
// sock_fd와 연결된 클라이언트에게 score_board를 보냄
void send_score(){
	FILE *fp = fopen("resource/score.txt", "r");
	struct Score score;
	int len_score_board = 0;

	fscanf(fp, "%d", &len_score_board);
	for(int i = 0; i < len_score_board; i++){
		fscanf(fp, "%s %d", score.name, &(score.score));
		fprintf(sock_score_fp, "%s %d\n", score.name, score.score);
	}

	fclose(fp);
}

// sock_fd와 연결된 클라이언트로부터 name과 score를 받아옴.
void receive_score(){
	struct Score score;

	fscanf(sock_score_fp, "%s %d", score.name, &(score.score));
	printf("%s %d\n", score.name,score.score);

	save_score(score);
}
// score를 파일에 저장함.
void save_score(struct Score score){
	FILE *fp = fopen("resource/score.txt", "r");
	struct Score score_board[11] = {{0,""}};
	int len_score_board = 0;
	int flag_update = 0;

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
		fp = fopen("resource/score.txt", "w");
		fprintf(fp, "%d\n",len_score_board);
		for(int i = 0; i < len_score_board; i++)
			fprintf(fp, "%s %d\n",score_board[i].name, score_board[i].score);
		fclose(fp);
	}
}
