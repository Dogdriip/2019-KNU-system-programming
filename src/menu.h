#pragma once
#include <stdio.h>
#include <fcntl.h>
#include <termio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include "single/single_menu.h"
#include "multi/multi_menu.h"

#define MENU_INTERVAL 5 // 메뉴 테두리가 스크린 가장자리로부터 얼마나 떨어지는가
#define MENU_TITLE_Y 6 // 제목의 Y좌표
#define MENU_SELECT_SINGLE_Y 19 // Single 선택의 Y 좌표
#define MENU_SELECT_MULTI_Y 23 // Multi 선택의 Y 좌표
#define MENU_SELECT_EXIT_Y 27 // Exit 선택의 Y 좌표

// ncurses에 정의되어 있는 전역 변수
// 기본 윈도우 : stdscr
// Screen Row : LINES 
// Screen Col : COLS

void tty_mode(int how);
void init_game();
void end_game();
void draw_menu();
int select_menu();
