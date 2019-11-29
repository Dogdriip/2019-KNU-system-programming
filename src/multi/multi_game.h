/*
 * 멀리 플레이어에 적용되는 함수들, 변수들
 */

#pragma once

#include <stdio.h>
#include <signal.h>
#include <curses.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include "../util/string.h"
#include "../single/endless.h"

// 유저가 플레이하는 게임 스크린의 높이, 너비, y, x
#define MULTI_GAME_WIN_HEIGHT LINES - (GAME_SCR_PADDING * 2) - TYPING_WIN_HEIGHT 
#define MULTI_GAME_WIN_WIDTH (COLS - (GAME_SCR_PADDING * 2)) / 2
#define MULTI_GAME_WIN_Y GAME_SCR_PADDING
#define MULTI_GAME_WIN_X GAME_SCR_PADDING

// 상대가 플레이하는 게임 스크린의 높이, 너비, y, x
#define MULTI_OTHER_WIN_HEIGHT LINES - (GAME_SCR_PADDING * 2) - TYPING_WIN_HEIGHT
#define MULTI_OTHER_WIN_WIDTH (COLS - (GAME_SCR_PADDING * 2)) / 2
#define MULTI_OTHER_WIN_Y GAME_SCR_PADDING
#define MULTI_OTHER_WIN_X GAME_SCR_PADDING + MULTI_GAME_WIN_WIDTH

#define MULTI_INIT_COMMUNICATION_C 1000

void start_multi_game(int);
