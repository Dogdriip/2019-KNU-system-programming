#include "single_menu.h"

// 메뉴는 기본 strscr에 그린당.
void start_single_menu(){
	int key = 0;

	draw_single_menu();

	while(1){
		key = select_single_menu();
		
		if (key == 1){
			// endless
			int score = single_endless_game();
			process_score(score, 0);

			return;
		}
		else if (key == 2){
			// time attack
			int score = single_time_attack_game();
			process_score(score, 1);

			return;
		}
		else{
			return;
		}
	}
}

// 방향키를 통해 메뉴를 선택할 수 있도록 함.
int select_single_menu(){
	int key = 0; // 누른 키
	int pos = 0; // 현재 '->'커서 위치. 범위는 0,1,2.  0은 싱글, 1은 멀티, 2는 Quit. return할 때 1 더해서 리턴
	int x[3] = {(COLS - strlen("1. ENDLESS")) / 2, (COLS - strlen("2. TIME ATTACK")) / 2, (COLS - strlen("3. Back")) / 2};
	int y[3] = {SINGLE_MENU_ENDLESS_Y, SINGLE_MENU_TIMEATTACK_Y, SINGLE_MENU_BACK_Y};

	while(1){
		mvprintw(y[pos], x[pos]-3,"->"); 
		refresh();

		key = getch();
		if (key == KEY_UP){
			mvprintw(y[pos], x[pos] - 3, "  ");
			if (pos > 0)
				pos--;
		}
		else if (key == KEY_DOWN){
			mvprintw(y[pos], x[pos] - 3, "  ");
			if (pos < 2)
				pos++;
		}
		else if (key == '\n'){
			mvprintw(y[pos], x[pos] - 3, "  ");
			return pos + 1;
		}
	}
}

// 이전에 strscr에 그려진 모든 것을 지우고, Single Game 선택 메뉴를 그림
void draw_single_menu(){
	clear();

	// 메뉴 테두리 구현...
	for(int i = MENU_INTERVAL; i < COLS - MENU_INTERVAL; i++)
		mvaddch(MENU_INTERVAL, i, '*');
	for(int i = MENU_INTERVAL; i < COLS - MENU_INTERVAL; i++)
		mvaddch(LINES - MENU_INTERVAL, i, '*');
	for(int i = MENU_INTERVAL; i < LINES - MENU_INTERVAL; i++)
		mvaddch(i, MENU_INTERVAL, '*');
	for(int i = MENU_INTERVAL; i < LINES - MENU_INTERVAL; i++)
		mvaddch(i, COLS - MENU_INTERVAL, '*');

	if (MENU_TITLE_Y + 10 < SINGLE_MENU_ENDLESS_Y)
		draw_title();

	mvprintw(SINGLE_MENU_ENDLESS_Y, (COLS - strlen("1. ENDLESS")) / 2, "1. ENDLESS");
	mvprintw(SINGLE_MENU_TIMEATTACK_Y, (COLS - strlen("2. TIME ATTACK")) / 2, "2. TIME ATTACK");
	mvprintw(SINGLE_MENU_BACK_Y, (COLS - strlen("3. Back")) / 2, "3. Back");

	refresh();
}
