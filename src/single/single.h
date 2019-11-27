/*
 * 싱글플레이 2개에 전반적으로 적용되는 함수들, 변수들
 */
// TODO: 일단 싱글에 몰아놨는데, 나중에 멀티에도 쓸 것들은 아예 공통적으로 쓸 수 있게 game.h 등으로 빼자

#pragma once

#include <stdio.h>
#include <signal.h>
#include <curses.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>

// 윈도우 3개 관련 상수
#define GAME_SCR_PADDING 5

#define TYPING_WIN_HEIGHT 5
#define TYPING_WIN_WIDTH COLS - (GAME_SCR_PADDING * 2)
#define TYPING_WIN_Y LINES - (GAME_SCR_PADDING * 2)
#define TYPING_WIN_X GAME_SCR_PADDING

#define GAME_WIN_HEIGHT LINES - (GAME_SCR_PADDING * 2) - TYPING_WIN_HEIGHT
#define GAME_WIN_WIDTH (COLS - (GAME_SCR_PADDING * 2)) * 2 / 3
#define GAME_WIN_Y GAME_SCR_PADDING
#define GAME_WIN_X GAME_SCR_PADDING

#define INFO_WIN_HEIGHT LINES - (GAME_SCR_PADDING * 2) - TYPING_WIN_HEIGHT
#define INFO_WIN_WIDTH (COLS - (GAME_SCR_PADDING * 2)) / 3
#define INFO_WIN_Y GAME_SCR_PADDING
#define INFO_WIN_X GAME_SCR_PADDING + GAME_WIN_WIDTH


// 게임플레이 관련
#define LIFE_INIT 5
#define CLOCK_INTERVAL 100
#define WORD_DROP_C_INIT 700
#define NEW_WORD_C_INIT 2000


// doubly linked list
typedef struct node {
    char word[40];  // word content
    int y, x;  // pos (row, col)
    struct node* llink;
    struct node* rlink;
} node;

node* get_node(char[], int, int);
void insert_node(node*, node*);
void delete_node(node*, node*);




