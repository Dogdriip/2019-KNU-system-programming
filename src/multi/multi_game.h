/*
 * 멀리 플레이어에 적용되는 함수들, 변수들
 */
#pragma once

#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <curses.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include "../util/string.h"

#define MULTI_GAME_SCR_PADDING 5

// 타이핑 윈도우
#define MULTI_TYPING_WIN_HEIGHT 5
#define MULTI_TYPING_WIN_WIDTH ((MY_COLS - (MULTI_GAME_SCR_PADDING * 2)) / 2)
#define MULTI_TYPING_WIN_Y (MY_LINES - MULTI_GAME_SCR_PADDING)
#define MULTI_TYPING_WIN_X (MY_COLS - MULTI_GAME_SCR_PADDING * 2)

// 유저가 플레이하는 게임 스크린의 높이, 너비, y, x
#define MULTI_GAME_WIN_HEIGHT (MY_LINES - (MULTI_GAME_SCR_PADDING * 2) - MULTI_TYPING_WIN_HEIGHT + 2) 
#define MULTI_GAME_WIN_WIDTH ((MY_COLS - (MULTI_GAME_SCR_PADDING * 2)) / 2)
#define MULTI_GAME_WIN_Y (MULTI_GAME_SCR_PADDING - 2)
#define MULTI_GAME_WIN_X MULTI_GAME_SCR_PADDING

// 상대가 플레이하는 게임 스크린의 높이, 너비, y, x
#define MULTI_OTHER_WIN_HEIGHT (MY_LINES - (MULTI_GAME_SCR_PADDING * 2) - MULTI_TYPING_WIN_HEIGHT + 2)
#define MULTI_OTHER_WIN_WIDTH ((MY_COLS - (MULTI_GAME_SCR_PADDING * 2)) / 2)
#define MULTI_OTHER_WIN_Y (MULTI_GAME_SCR_PADDING - 2)
#define MULTI_OTHER_WIN_X (MULTI_GAME_SCR_PADDING + MULTI_GAME_WIN_WIDTH)

// 멀티 스크린 커뮤니케이션 interval 
#define MULTI_INIT_COMMUNICATION_C 100

void start_multi_game(int);
