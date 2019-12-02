#include "single_gameover.h"

void process_score(int elapsed_time){
	char name[4] = "";
	int char_pos[3] = {0};
	int key = 0, pos = 0;
	int score_board[10];
	char name_board[10][4];

	noecho();

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


	while(1){
		for(int i = 0; i <= 2; i++){
			for(int j = -2; j <= 2; j++){
				if (j == 0)
					mvprintw(LINES / 2 - 1 + i, COLS / 2, "%c", char_pos[i] + 'A');
				else
					mvprintw(LINES / 2 - 1 + i, COLS / 2 + j, " ");
			}
		}
		mvprintw(LINES / 2 - 1 + pos, COLS / 2 - 2, "<-");
		mvprintw(LINES / 2 - 1 + pos, COLS / 2 + 1, "->");

		refresh();

		key = getch();
		if (key == '\n')
			break;
		else if (key == KEY_UP){
			pos--;
			if (pos < 0)
				pos = 0;
		}
		else if (key == KEY_DOWN){
			pos++;
			if (pos > 2)
				pos = 2;
		}
		else if (key == KEY_LEFT){
			char_pos[pos]--;
			if (char_pos[pos] < 0)
				char_pos[pos] = 0;
		}
		else if (key == KEY_RIGHT){
			char_pos[pos]++;
			if (char_pos[pos] > 25)
				char_pos[pos] = 25;
		}
	}
	
	for(int i = 0; i < 3; i++)
		name[i] = char_pos[i] + 'A';

	int socket_id = open_score_server();

	if (send_score(socket_id, elapsed_time, name) == -1)
		return;

	int len = receive_score(socket_id, score_board, name_board);
	if (len == -1)
		return;

	for(int i = -1; i <= 1; i++)
		mvprintw(LINES / 2 + i, COLS / 2 - 2, "     ");

	mvprintw(LINES / 2, (COLS - strlen("Score Board")) / 2, "Score Board");
	for(int i = 0; i < len; i++)
		mvprintw(LINES / 2 + 1 + i, COLS / 2 - 4, "%s : %d",name_board[i], score_board[i]);
	mvprintw(LINES / 2 + 12, (COLS - strlen("Please Enter Key...")) / 2, "Please Enter Key...");

	refresh();
	getch();

}
