#include <stdio.h>
#include <signal.h>
#include <curses.h>
#include <string.h>
#include <sys/time.h>

#define MENU_INTERVAL 5

////////////////////////////////////////////////////////
// WINDOW들은 전역변수로 선언해야 한다
// window update 함수들은 매 tick마다 이루어져야 한다.
// word drop이랑 체크 같은 건 trigger에서 해 준다만

WINDOW* game_win;  // 좌상단 가장 큰 게임 메인창
WINDOW* info_win;  // 우상단 현재 게임 정보 출력창
WINDOW* typing_win;  // 하단 유저 입력창



void update_info_win(int life, int score) {
    wmove(info_win, 2, 2);
    wprintw(info_win, "LIFE: %d", life);
    wmove(info_win, 3, 2);
    wprintw(info_win, "SCORE: %d", score);
}




////////////////////////////////////////////////////////
// 테스트를 위한 파일
// timer.c에 있는 기능 그대로 가져옴
// 일단 word drop만 임의로 해 보자
// 걍 여기서 완성해서 나중에 모듈로 분리하는 게 맞을 듯

#define INTERVAL 100
#define WORD_DROP_C_INIT 2000
// #define NEW_WORD_C_INIT 2000

int elapsed_time;
int word_drop_c;
// int new_word_c;


void trigger() {
    // 매 trigger마다 변수 갱신
    elapsed_time += INTERVAL;
    word_drop_c -= INTERVAL;
    // new_word_c -= INTERVAL;

    // info_win에 정보 갱신 (LIFE, SCORE)
    update_info_win(100, elapsed_time);

    // 각 window를 refresh
    wrefresh(game_win);
    wrefresh(info_win);
    wrefresh(typing_win);
    // typing window는 refresh하면 안 되는 것 아닌가?


    if (word_drop_c == 0) {
        
        


        word_drop_c = WORD_DROP_C_INIT;
    }
}

// 주어진 n_msecs (밀리초)로 타이머 설정 (INTERVAL == 100으로 설정되어야 함)
int set_ticker(int n_msecs) {
    struct itimerval new_timeset;
    long n_sec, n_usecs;
    n_sec = n_msecs / 1000;
    n_usecs = (n_msecs % 1000) * 1000L;

    new_timeset.it_interval.tv_sec = n_sec;
    new_timeset.it_interval.tv_usec = n_usecs;
    new_timeset.it_value.tv_sec = n_sec;
    new_timeset.it_value.tv_usec = n_usecs;
    
    return setitimer(ITIMER_REAL, &new_timeset, NULL);
}

void init_timer() {
    elapsed_time = 0;
    word_drop_c = WORD_DROP_C_INIT;
    // new_word_c = NEW_WORD_C_INIT;

    set_ticker(INTERVAL);
    signal(SIGALRM, trigger);
}

























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

    char input_str[40];
    int input_len = 0;
    int i;


    init_timer();



    while (strcmp(input_str, "quit")) {
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
