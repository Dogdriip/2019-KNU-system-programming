#pragma once
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include "../multi/multi_menu.h"

#define PORTNUM_STRING 8181
#define PORTNUM_SCORE 8281
#define PORTNUM_MULTI 8182
#define SERVERADDRESS "155.230.52.58"

// multi의 연결을 관장한다.
void* multi_connection(void* m);

// 점수를 서버로 보낸다. score : 점수(초단위), name : 이름
// mode == 0 endless, mode == 1 timeattack
int send_score(int socket_id, int mode, int score, char *name);

// 서버로부터 점수를 받아옴. 상위 10등까지의 점수를 받아온다.
// score 배열과 name 배열에 각각 점수와 이름이 들어감.
// index : 0부터 시작 
// mode == 0 endless, mode == 1 timeattack
int receive_score(int socket_id, int mode, int score[], char name[][4]);

// score server를 열어줌
int open_score_server();

// score server를 닫아줌
void close_score_server();
