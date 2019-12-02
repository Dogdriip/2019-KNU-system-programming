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


	if (MENU_TITLE_Y + 10 < LINES / 2 - 5)
		draw_title();

	// 이름 입력
	mvprintw(LINES / 2 - 4, (COLS - strlen("Enter Your Name")) / 2, "Enter Your Name");
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

	// 서버랑 점수를 주고, 받음.
	int socket_id = open_score_server();

	if (send_score(socket_id, elapsed_time, name) == -1)
		return;

	int len = receive_score(socket_id, score_board, name_board);
	if (len == -1)
		return;

	mvprintw(LINES / 2 - 4, (COLS - strlen("Enter Your Name")) / 2, "               ");
	for(int i = -1; i <= 1; i++)
		mvprintw(LINES / 2 + i, COLS / 2 - 2, "     ");

	mvprintw(LINES / 2 - 4, (COLS - strlen("Score Board")) / 2, "Score Board");
	for(int i = 0; i < len; i++)
		mvprintw(LINES / 2 - 2 + i, COLS / 2 - 4, "%s : %d",name_board[i], score_board[i]);
	mvprintw(LINES / 2 + 9, (COLS - strlen("Please Enter Key...")) / 2, "Please Enter Key...");

	refresh();
	getch();

}
