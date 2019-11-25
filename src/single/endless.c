// 헤더랑 main은 나중에 빼고 함수로 리팩토링하면 된다

#include <stdio.h>
#include <signal.h>
#include "../menu.h"

WINDOW* game_win;  // 좌상단 가장 큰 게임 메인창
WINDOW* info_win;  // 우상단 현재 게임 정보 출력창
WINDOW* typing_win;  // 하단 유저 입력창

int main() {
    init_game();  // 단어 리스트 초기화
    init_timer();  // 타이머 설정, 시그널 핸들러 설치, 경과 시간 초기화

    game_win = newwin(LINES - (MENU_INTERVAL * 2) - 3, (COLS - (MENU_INTERVAL * 2)) * 2 / 3, MENU_INTERVAL, MENU_INTERVAL);
    box(game_win, '*', '*');
    wrefresh(game_win);

    info_win = newwin(LINES - (MENU_INTERVAL * 2) - 3, (COLS - (MENU_INTERVAL * 2)) / 3, MENU_INTERVAL, MENU_INTERVAL + (COLS - (MENU_INTERVAL * 2)) * 2 / 3);
    box(info_win, '*', '*');
    wrefresh(info_win);

    typing_win = newwin(3, COLS - (MENU_INTERVAL * 2), LINES - 8, MENU_INTERVAL);
    box(typing_win, '*', '*');
    wrefresh(typing_win);

    /*while (1) {
        
    }*/

    return 0;
}