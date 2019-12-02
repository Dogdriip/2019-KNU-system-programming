#include "main_menu.h"

int main(){
	int key = 0;
	initscr();
	clear();

	draw_main_menu();
	if (init_program() == -1){
		while(getch() != '\n');
		end_program();
		endwin();
		return 0;
	}
	draw_main_menu();

	while(key != 3){
		// key) 1: Single, 2: Multi, 3: Exit
		key = select_main_menu();

		if (key == 1){
			start_single_menu();
		}
		else if (key == 2){
			start_multi_menu();
		}
		draw_main_menu();
	}

	end_program();
	endwin();

	return 0;
}

void tty_mode(int how){
	static struct termios original_mode;
	static int original_flags;
	static int stored = 0;

	if (how == 0){
		tcgetattr(0,&original_mode);
		original_flags = fcntl(0, F_GETFL);
		stored = 1;
	}
	else if ( stored ){
		tcsetattr(0, TCSANOW, &original_mode);
		original_flags = fcntl(0, F_SETFL, original_flags);
	}
}

// program 시작시 초기화
int init_program(){
	tty_mode(0);
	noecho();
	srand((long)time(NULL));
	keypad(stdscr, TRUE);

	if (string_init(0) == -1){ // 0이면 서버에서, 1이면 로컬에서
		clear();
		mvprintw(LINES/2, COLS/2 - 10, "game error. please restart");
		return -1;
	}

	mvprintw(MENU_SELECT_SINGLE_Y, (COLS - strlen("              ")) / 2, "               "); 
	mvprintw(MENU_SELECT_MULTI_Y, (COLS - strlen("               ")) / 2, "               "); 
	mvprintw(MENU_SELECT_EXIT_Y, (COLS - strlen("          ")) / 2, "          "); 
	refresh();
	int i = 0;
	char message[3][30] = {"Server Connecting.  ", "Server Connecting.. ", "Server Connecting..."};
	while(string_init(0) == -1){
		mvprintw(MENU_SELECT_SINGLE_Y, (COLS - strlen(message[2])) / 2, message[i]);
		i = (i + 1) % 3;
		refresh();
		sleep(1);
	}

	curs_set(0);

	return 0;
}

// program 종료시 초기화
void end_program(){
	echo();
	tty_mode(1);
}

// 방향키를 통해 메뉴를 선택할 수 있도록 함.
int select_main_menu(){
	int key = 0; // 누른 키
	int pos = 0; // 현재 '->'커서 위치. 범위는 0,1,2.  0은 싱글, 1은 멀티, 2는 Quit. return할 때 1 더해서 리턴
	int x[3] = {(COLS - strlen("1. Single Game")) / 2, (COLS - strlen("2. Multi Game")) / 2, (COLS - strlen("3. Exit")) / 2};
	int y[3] = {MENU_SELECT_SINGLE_Y, MENU_SELECT_MULTI_Y, MENU_SELECT_EXIT_Y};

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

// 타이틀 그려줌
void draw_title(){
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

	refresh();
}

// 이전에 strscr에 그려진 모든 것을 지우고, 메인 메뉴를 그림
void draw_main_menu(){
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

	if (MENU_TITLE_Y + 10 < MENU_SELECT_SINGLE_Y)
		draw_title();

	mvprintw(MENU_SELECT_SINGLE_Y, (COLS - strlen("1. Single Game")) / 2, "1. Single Game"); 
	mvprintw(MENU_SELECT_MULTI_Y, (COLS - strlen("2. Multi Game")) / 2, "2. Multi Game"); 
	mvprintw(MENU_SELECT_EXIT_Y, (COLS - strlen("3. Exit")) / 2, "3. Exit"); 


	refresh();
}
