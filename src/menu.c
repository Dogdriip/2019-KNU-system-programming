#include <stdio.h>
#include <string.h>
#include <curses.h>

#define MENU_INTERVAL 5
#define MENU_TITLE_Y 6
#define MENU_SELECT_SINGLE_Y 19
#define MENU_SELECT_MULTI_Y 23
#define MENU_SELECT_EXIT_Y 27

int screen_col = 0, screen_row = 0;

void init_game();
void draw_menu();
int select_menu();

int main(){
	int key = 0;
	initscr();
	clear();
	keypad();

	draw_menu();
	
	while(key != 3){
		key = select_menu();
	
	}
	endwin();
}

void init_game(){
	getmaxyx(stdscr, screen_row, screen_col); 
}

int select_menu(){
	int key = 0;
	int pos = 1;
	int x[3] = {(screen_col - strlen("1. Single Game")) / 2, (screen_col - strlen("2. Multi Game")) / 2, (screen_col - strlen("3. Exit")) / 2};
	int y[3] = {MENU_SELECT_SINGLE_Y, MENU_SELECT_MULTI_Y, MENU_SELECT_EXIT_Y};

	move(MENU_SELECT_SINGLE_Y, x_1); 

	refresh();

	key = getc();

	return pos;
}


void draw_menu(){
	for(int i = MENU_INTERVAL; i < screen_col - MENU_INTERVAL; i++)
		mvaddch(MENU_INTERVAL, i, '*');
	for(int i = MENU_INTERVAL; i < screen_col - MENU_INTERVAL; i++)
		mvaddch(screen_row - MENU_INTERVAL, i, '*');
	for(int i = MENU_INTERVAL; i < screen_row - MENU_INTERVAL; i++)
		mvaddch(i, MENU_INTERVAL, '*');
	for(int i = MENU_INTERVAL; i < screen_row - MENU_INTERVAL; i++)
		mvaddch(i, screen_col - MENU_INTERVAL, '*');


	mvprintw(MENU_TITLE_Y, (screen_col - strlen(" /$$$$$$$              /$$     /$$     /$$                 /$$$$$$$$                  /$$                    ")) / 2, " /$$$$$$$              /$$     /$$     /$$                 /$$$$$$$$                  /$$                    ");         
	mvprintw(MENU_TITLE_Y+1, (screen_col - strlen("| $$__  $$            | $$    | $$    | $$                |__  $$__/                 |__/                    "))/2, "| $$__  $$            | $$    | $$    | $$                |__  $$__/                 |__/                    "); 
	mvprintw(MENU_TITLE_Y+2, (screen_col - strlen("| $$  \\ $$  /$$$$$$  /$$$$$$ /$$$$$$  | $$  /$$$$$$          | $$ /$$   /$$  /$$$$$$  /$$ /$$$$$$$   /$$$$$$ "))/2, "| $$  \\ $$  /$$$$$$  /$$$$$$ /$$$$$$  | $$  /$$$$$$          | $$ /$$   /$$  /$$$$$$  /$$ /$$$$$$$   /$$$$$$ ");
	mvprintw(MENU_TITLE_Y+3, (screen_col - strlen("| $$$$$$$  |____  $$|_  $$_/|_  $$_/  | $$ /$$__  $$         | $$| $$  | $$ /$$__  $$| $$| $$__  $$ /$$__  $$"))/2, "| $$$$$$$  |____  $$|_  $$_/|_  $$_/  | $$ /$$__  $$         | $$| $$  | $$ /$$__  $$| $$| $$__  $$ /$$__  $$");
	mvprintw(MENU_TITLE_Y+4, (screen_col - strlen("| $$__  $$  /$$$$$$$  | $$    | $$    | $$| $$$$$$$$         | $$| $$  | $$| $$  \\ $$| $$| $$  \\ $$| $$  \\ $$")) / 2, "| $$__  $$  /$$$$$$$  | $$    | $$    | $$| $$$$$$$$         | $$| $$  | $$| $$  \\ $$| $$| $$  \\ $$| $$  \\ $$");
	mvprintw(MENU_TITLE_Y+5, (screen_col - strlen("| $$  \\ $$ /$$__  $$  | $$ /$$| $$ /$$| $$| $$_____/         | $$| $$  | $$| $$  | $$| $$| $$  | $$| $$  | $$"))/2, "| $$  \\ $$ /$$__  $$  | $$ /$$| $$ /$$| $$| $$_____/         | $$| $$  | $$| $$  | $$| $$| $$  | $$| $$  | $$");
	mvprintw(MENU_TITLE_Y+6, (screen_col - strlen("| $$$$$$$/|  $$$$$$$  |  $$$$/|  $$$$/| $$|  $$$$$$$         | $$|  $$$$$$$| $$$$$$$/| $$| $$  | $$|  $$$$$$$")) /2,"| $$$$$$$/|  $$$$$$$  |  $$$$/|  $$$$/| $$|  $$$$$$$         | $$|  $$$$$$$| $$$$$$$/| $$| $$  | $$|  $$$$$$$");
	mvprintw(MENU_TITLE_Y+7, (screen_col - strlen("|_______/  \\_______/   \\___/   \\___/  |__/ \\_______/         |__/ \\____  $$| $$____/ |__/|__/  |__/ \\____  $$"))/2, "|_______/  \\_______/   \\___/   \\___/  |__/ \\_______/         |__/ \\____  $$| $$____/ |__/|__/  |__/ \\____  $$");
	mvprintw(MENU_TITLE_Y+8, (screen_col - strlen("                                                                  /$$  | $$| $$                     /$$  \\ $$"))/2, "                                                                  /$$  | $$| $$                     /$$  \\ $$");
	mvprintw(MENU_TITLE_Y+9, (screen_col - strlen("                                                                 |  $$$$$$/| $$                    |  $$$$$$/"))/2, "                                                                 |  $$$$$$/| $$                    |  $$$$$$/");
	mvprintw(MENU_TITLE_Y+10, (screen_col - strlen("                                                                  \\______/ |__/                     \\______/ "))/2, "                                                                  \\______/ |__/                     \\______/ ");

	mvprintw(MENU_SELECT_SINGLE_Y, (screen_col - strlen("1. Single Game")) / 2, "1. Single Game"); 
	mvprintw(MENU_SELECT_MULTI_Y, (screen_col - strlen("2. Multi Game")) / 2, "2. Multi Game"); 
	mvprintw(MENU_SELECT_EXIT_Y, (screen_col - strlen("3. Exit")) / 2, "3. Exit"); 


	refresh();
}
