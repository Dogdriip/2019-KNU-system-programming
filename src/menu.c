#include "menu.h"

int main(){
	int key = 0;
	initscr();
	clear();
	keypad(stdscr, TRUE);

	init_game();
	draw_menu();

	while(key != 3){
		// key) 1: Single, 2: Multi, 3: Exit
		key = select_menu();
	}

	end_game();
	endwin();
}

// ECHO를 끔
void set_noecho_mode(){
	struct termios ttystate;

	tcgetattr(0, &ttystate);
	ttystate.c_lflag &= ~ECHO;

	tcsetattr(0, TCSANOW,  &ttystate);
}

// 터미널 설정을 저장해두고, 복구함
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
void init_game(){
	tty_mode(0);
	set_noecho_mode();
	srand((long)time(NULL));

	curs_set(0);
}

// program 종료시 초기화
void end_game(){
	tty_mode(1);
}

// 방향키를 통해 메뉴를 선택할 수 있도록 함.
int select_menu(){
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

// 메인 메뉴를 그림
void draw_menu(){
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

	mvprintw(MENU_SELECT_SINGLE_Y, (COLS - strlen("1. Single Game")) / 2, "1. Single Game"); 
	mvprintw(MENU_SELECT_MULTI_Y, (COLS - strlen("2. Multi Game")) / 2, "2. Multi Game"); 
	mvprintw(MENU_SELECT_EXIT_Y, (COLS - strlen("3. Exit")) / 2, "3. Exit"); 


	refresh();
}
