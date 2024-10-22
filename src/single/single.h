/*
 * 싱글플레이 2개에 전반적으로 적용되는 함수들, 상수들
 */

#pragma once

#include <stdio.h>
#include <signal.h>
#include <curses.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>
#include "../util/game.h"
#include "../util/string.h"

// 윈도우 3개 관련 상수
#define TYPING_WIN_HEIGHT 5
#define TYPING_WIN_WIDTH (COLS - (GAME_SCR_PADDING * 2))
#define TYPING_WIN_Y (LINES - (GAME_SCR_PADDING * 2))
#define TYPING_WIN_X GAME_SCR_PADDING

#define GAME_WIN_HEIGHT (LINES - (GAME_SCR_PADDING * 2) - TYPING_WIN_HEIGHT)
#define GAME_WIN_WIDTH ((COLS - (GAME_SCR_PADDING * 2)) * 2 / 3)
#define GAME_WIN_Y GAME_SCR_PADDING
#define GAME_WIN_X GAME_SCR_PADDING

#define INFO_WIN_HEIGHT (LINES - (GAME_SCR_PADDING * 2) - TYPING_WIN_HEIGHT)
#define INFO_WIN_WIDTH ((COLS - (GAME_SCR_PADDING * 2)) / 3)
#define INFO_WIN_Y GAME_SCR_PADDING
#define INFO_WIN_X (GAME_SCR_PADDING + GAME_WIN_WIDTH)

#define GAMEOVER_WIN_HEIGHT (LINES - (GAME_SCR_PADDING * 2))
#define GAMEOVER_WIN_WIDTH (COLS - (GAME_SCR_PADDING * 2))
#define GAMEOVER_WIN_Y GAME_SCR_PADDING
#define GAMEOVER_WIN_X GAME_SCR_PADDING

// linked list 관련
node* get_node(char[], int, int);
void insert_node(node*, node*);
void delete_node(node*, node*);


// window 관련
void prepare_windows();
void update_info_win(int, int, int);
void update_game_win(node*);

// 게임플레이
void gameover();
void drop_word(node*);
void add_new_word(node*);
void input_handler(node*, char[]);
int set_ticker(int);
void trigger();
void init_timer();
void init_game();



//////////////////////////////////////////////////// time_attack
// linked list 관련
node* ta_get_node(char[], int, int);
void ta_insert_node(node*, node*);
void ta_delete_node(node*, node*);


// window 관련
void ta_prepare_windows();
void ta_update_info_win(int, int);
void ta_update_game_win(node*);

// 게임플레이
void ta_gameover();
void ta_drop_word(node*);
void ta_add_new_word(node*);
void ta_input_handler(node*, char[]);
int ta_set_ticker(int);
void ta_trigger();
void ta_init_timer();
void ta_init_game();
