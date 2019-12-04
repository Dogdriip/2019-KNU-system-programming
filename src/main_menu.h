#pragma once
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <time.h>
#include "single/single_menu.h"
#include "multi/multi_menu.h"
#include "util/string.h"

#define MENU_INTERVAL 5 // 메뉴 테두리가 스크린 가장자리로부터 얼마나 떨어지는가
#define MENU_TITLE_Y 8 // 제목의 Y좌표
#define MENU_SELECT_SINGLE_Y (LINES / 2) // Single 선택의 Y 좌표
#define MENU_SELECT_MULTI_Y (LINES / 2 + 4) // Multi 선택의 Y 좌표
#define MENU_SELECT_EXIT_Y (LINES / 2 + 8) // Exit 선택의 Y 좌표

// ncurses에 정의되어 있는 전역 변수
// 기본 윈도우 : stdscr
// Screen Row : LINES 
// Screen Col : COLS

void tty_mode(int how); // 프로그램 실행 전 터미널 설정을 저장해주고, 복구해주는 함수. 사실 필요없을 듯
int init_program(); // Program 시작시 초기화 및 필요한 작업을 해주는 함수
void end_program(); // Program 종료시 실행...
void draw_main_menu(); // strscr에 그려진 모든 텍스트를 지우고, 메인 메뉴를 출력해줌
int select_main_menu(); // 방향키와 엔터로 메뉴를 선택하도록 하는 함수
void draw_title(); // 타이틀 그려줌
