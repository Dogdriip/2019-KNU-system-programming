#include <stdio.h>
#include <signal.h>
#include <curses.h>
#include <string.h>

#define MENU_INTERVAL 5

WINDOW* game_win;  // 좌상단 가장 큰 게임 메인창
WINDOW* info_win;  // 우상단 현재 게임 정보 출력창
WINDOW* typing_win;  // 하단 유저 입력창

int main() {

    initscr();
    clear();
    refresh();

    game_win = newwin(LINES - (MENU_INTERVAL * 2) - 5, (COLS - (MENU_INTERVAL * 2)) * 2 / 3, MENU_INTERVAL, MENU_INTERVAL);
    box(game_win, '*', '*');

    info_win = newwin(LINES - (MENU_INTERVAL * 2) - 5, (COLS - (MENU_INTERVAL * 2)) / 3, MENU_INTERVAL, MENU_INTERVAL + (COLS - (MENU_INTERVAL * 2)) * 2 / 3);
    box(info_win, '*', '*');

    typing_win = newwin(5, COLS - (MENU_INTERVAL * 2), LINES - 10, MENU_INTERVAL);
    box(typing_win, '*', '*');

    refresh();

    char *input_str;
    int input_len = 0;
    int i;

    while (strcmp(input_str, "quit")) {
        // info_win에 정보 표시
        wmove(info_win, 2, 2);
        wprintw(info_win, "LIFE: %d", 0);
        wmove(info_win, 3, 2);
        wprintw(info_win, "SCORE: %d", 0);

        // 각 window를 refresh
        wrefresh(game_win);
        wrefresh(info_win);
        wrefresh(typing_win);



        // input 받자
        wmove(typing_win, 2, 2);
        wgetstr(typing_win, input_str);
        input_len = strlen(input_str);

        // input 받았으면 공백으로 채우자
        for (i = 2; i < input_len + 2; i++) {
            wmove(typing_win, 2, i);
            wprintw(typing_win, " ");
        }
    }
    
    wclear(game_win);
    wclear(info_win);
    wclear(typing_win);

    delwin(game_win);
    delwin(info_win);
    delwin(typing_win);

    endwin();

    return 0;
}
