#include "multi_menu.h"

// connecting 연결 중.
int multi_connecting(){
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

	pthread_t t1;
	int flag = 0;
	
	//multi_connection은 util/connection.c의 함수
	pthread_create(&t1, NULL, multi_connection, (void*)&flag);

	mvprintw(MULTI_MENU_MATCH_Y + 5, (COLS - strlen("Loading...")) / 2, "Loading...");
	mvprintw(MULTI_MENU_BACK_Y, (COLS - strlen("-> Back")) / 2, "-> Back");

	nodelay(stdsrc, TRUE);
	while(1){
		int c = getch();
		if (c == '\n'){
			pthread_cancel(t1);	
			return -1;
		}
		if (flag == 1)
			break;
	}

	return 1;
}

// 메뉴는 기본 strscr에 그린당.
void start_multi_menu(){
	int key = 0;

	draw_multi_menu();

	while(1){
		key = select_multi_menu();
		mvprintw(MULTI_MENU_BACK_Y, (COLS - strlen("2. Back")) / 2, "2. Back");
			
			if (key == 1){
				int flag = multi_connecting();
				if (flag == -1)
					draw_multi_menu();
				else if (flag == 1){
					mvprintw(MULTI_MENU_BACK_Y - 1, COLS/2, "Wa! Sans!");
					refresh();
					return;
				}
				else
					return;
			}
			else if (key == 2){
				return;
			}
		}
	}

// 방향키를 통해 메뉴를 선택할 수 있도록 함.
int select_multi_menu(){
	int key = 0; // 누른 키
	int pos = 0; // 현재 '->'커서 위치. 범위는 0,1,2.  0은 싱글, 1은 멀티, 2는 Quit. return할 때 1 더해서 리턴
	int x[2] = {(COLS - strlen("1. Match")) / 2, (COLS - strlen("2. Back")) / 2};
	int y[2] = {MULTI_MENU_MATCH_Y, MULTI_MENU_BACK_Y};

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
			if (pos < 1)
				pos++;
		}
		else if (key == '\n'){
			mvprintw(y[pos], x[pos] - 3, "  ");
			return pos + 1;
		}
	}
}

// 이전에 strscr에 그려진 모든 것을 지우고, Multi Game 선택 메뉴를 그림
void draw_multi_menu(){
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

	mvprintw(MENU_TITLE_Y, (COLS - strlen(" /$$$$$$$              /$$     /$$     /$$                 /$$$$$$$$                  /$$                    ")) / 2, " /$$$$$$$              /$$     /$$     /$$                 /$$$$$$$$                  /$$                    ");         
	mvprintw(MENU_TITLE_Y+1, (COLS - strlen("| $$__  $$            | $$    | $$    | $$                |__  $$__/                 |__/                    "))/2, "| $$__  $$            | $$    | $$    | $$                |__  $$__/                 |__/                    "); 
	mvprintw(MENU_TITLE_Y+2, (COLS - strlen("| $$  \\ $$  /$$$$$$  /$$$$$$ /$$$$$$  | $$  /$$$$$$          | $$ /$$   /$$  /$$$$$$  /$$ /$$$$$$$   /$$$$$$ "))/2, "| $$  \\ $$  /$$$$$$  /$$$$$$ /$$$$$$  | $$  /$$$$$$          | $$ /$$   /$$  /$$$$$$  /$$ /$$$$$$$   /$$$$$$ ");
	mvprintw(MENU_TITLE_Y+3, (COLS - strlen("| $$$$$$$  |____  $$|_  $$_/|_  $$_/  | $$ /$$__  $$         | $$| $$  | $$ /$$__  $$| $$| $$__  $$ /$$__  $$"))/2, "| $$$$$$$  |____  $$|_  $$_/|_  $$_/  | $$ /$$__  $$         | $$| $$  | $$ /$$__  $$| $$| $$__  $$ /$$__  $$");
	mvprintw(MENU_TITLE_Y+4, (COLS - strlen("| $$__  $$  /$$$$$$$  | $$    | $$    | $$| $$$$$$$$         | $$| $$  | $$| $$  \\ $$| $$| $$  \\ $$| $$  \\ $$")) / 2, "| $$__  $$  /$$$$$$$  | $$    | $$    | $$| $$$$$$$$         | $$| $$  | $$| $$  \\ $$| $$| $$  \\ $$| $$  \\ $$");
	mvprintw(MENU_TITLE_Y+5, (COLS - strlen("| $$  \\ $$ /$$__  $$  | $$ /$$| $$ /$$| $$| $$_____/         | $$| $$  | $$| $$  | $$| $$| $$  | $$| $$  | $$"))/2, "| $$  \\ $$ /$$__  $$  | $$ /$$| $$ /$$| $$| $$_____/         | $$| $$  | $$| $$  | $$| $$| $$  | $$| $$  | $$");
	mvprintw(MENU_TITLE_Y+6, (COLS - strlen("| $$$$$$$/|  $$$$$$$  |  $$$$/|  $$$$/| $$|  $$$$$$$         | $$|  $$$$$$$| $$$$$$$/| $$| $$  | $$|  $$$$$$$")) /2,"| $$$$$$$/|  $$$$$$$  |  $$$$/|  $$$$/| $$|  $$$$$$$         | $$|  $$$$$$$| $$$$$$$/| $$| $$  | $$|  $$$$$$$");
	mvprintw(MENU_TITLE_Y+7, (COLS - strlen("|_______/  \\_______/   \\___/   \\___/  |__/ \\_______/         |__/ \\____  $$| $$____/ |__/|__/  |__/ \\____  $$"))/2, "|_______/  \\_______/   \\___/   \\___/  |__/ \\_______/         |__/ \\____  $$| $$____/ |__/|__/  |__/ \\____  $$");
	mvprintw(MENU_TITLE_Y+8, (COLS - strlen("                                                                  /$$  | $$| $$                     /$$  \\ $$"))/2, "                                                                  /$$  | $$| $$                     /$$  \\ $$");
	mvprintw(MENU_TITLE_Y+9, (COLS - strlen("                                                                 |  $$$$$$/| $$                    |  $$$$$$/"))/2, "                                                                 |  $$$$$$/| $$                    |  $$$$$$/");
	mvprintw(MENU_TITLE_Y+10, (COLS - strlen("                                                                  \\______/ |__/                     \\______/ "))/2, "                                                                  \\______/ |__/                     \\______/ ");

	mvprintw(MULTI_MENU_MATCH_Y, (COLS - strlen("1. Match")) / 2, "1. Match");
	mvprintw(MULTI_MENU_BACK_Y, (COLS - strlen("2. Back")) / 2, "2. Back");

	refresh();
}