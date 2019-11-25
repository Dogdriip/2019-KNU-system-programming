#include <curses.h>
#include <stdio.h>
#include <string.h>

int main(){
	initscr();
	clear();

	move(10,10);

	printw("asdasdasd");
	
	refresh();
	getch();

	WINDOW* win1 = newwin(LINES - 13, (COLS - 10) * 2 / 3, 5, 5);
	box(win1, '*', '*');
	wrefresh(win1);

	WINDOW* win2 = newwin(LINES - 13, (COLS - 10) / 3, 5, 5 + (COLS - 10) * 2 / 3);
	box(win2, '*', '*');
	wrefresh(win2);

	WINDOW* win3 = newwin(3, (COLS - 10), LINES - 8, 5);
	box(win3, '*', '*');
	wrefresh(win3);

	getch();

	char string[30] = "asd";
	while(strcmp(string, "quit") != 0){
		wmove(win3, 1, 1);
		wgetstr(win3, string);

	}

	endwin();

	return 0;
}
