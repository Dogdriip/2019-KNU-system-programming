#include <stdio.h>
#include <signal.h>
#include <curses.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>

#define GAME_SCR_PADDING 5

#define TYPING_WIN_HEIGHT 5
#define TYPING_WIN_WIDTH COLS - (GAME_SCR_PADDING * 2)
#define TYPING_WIN_Y LINES - (GAME_SCR_PADDING * 2)
#define TYPING_WIN_X GAME_SCR_PADDING

#define GAME_WIN_HEIGHT LINES - (GAME_SCR_PADDING * 2) - TYPING_WIN_HEIGHT
#define GAME_WIN_WIDTH (COLS - (GAME_SCR_PADDING * 2)) * 2 / 3
#define GAME_WIN_Y GAME_SCR_PADDING
#define GAME_WIN_X GAME_SCR_PADDING

#define INFO_WIN_HEIGHT LINES - (GAME_SCR_PADDING * 2) - TYPING_WIN_HEIGHT
#define INFO_WIN_WIDTH (COLS - (GAME_SCR_PADDING * 2)) / 3
#define INFO_WIN_Y GAME_SCR_PADDING
#define INFO_WIN_X GAME_SCR_PADDING + GAME_WIN_WIDTH


/////////////////////////////////////////////////////////////// doubly linked list
typedef struct node {
    char word[40];  // word content
    int y, x;  // pos (row, col)
    struct node* llink;
    struct node* rlink;
} node;
node* list_header;

node* get_node(char word[], int y, int x) {
    node* tmp = (node*)malloc(sizeof(*tmp));
    
    strcpy(tmp->word, word);
    tmp->y = y; tmp->x = x; tmp->llink = NULL; tmp->rlink = NULL;

    return tmp;
}

void insert_node(node* destnode, node* newnode) {
	newnode->llink = destnode;
	newnode->rlink = destnode->rlink;
	destnode->rlink->llink = newnode;
	destnode->rlink = newnode;
}

void delete_node(node* destnode, node* deleted) {
	if (destnode == deleted) {
		printf("Deletion of header node not permitted.\n");
	} else {
		deleted->llink->rlink = deleted->rlink;
		deleted->rlink->llink = deleted->llink;
		free(deleted);
	}
}
/////////////////////////////////////////////////////////////// doubly linked list






////////////////////////////////////////////////////////
// WINDOW들은 전역변수로 선언해야 한다
// window update 함수들은 매 tick마다 이루어져야 한다.
// word drop이랑 체크 같은 건 trigger에서 해 준다만

WINDOW* game_win;  // 좌상단 가장 큰 게임 메인창
WINDOW* info_win;  // 우상단 현재 게임 정보 출력창
WINDOW* typing_win;  // 하단 유저 입력창

void prepare_windows() {
    game_win = newwin(GAME_WIN_HEIGHT, GAME_WIN_WIDTH, GAME_WIN_Y, GAME_WIN_X);
    box(game_win, '*', '*');

    info_win = newwin(INFO_WIN_HEIGHT, INFO_WIN_WIDTH, INFO_WIN_Y, INFO_WIN_X);
    box(info_win, '*', '*');

    typing_win = newwin(TYPING_WIN_HEIGHT, TYPING_WIN_WIDTH, TYPING_WIN_Y, TYPING_WIN_X);
    box(typing_win, '*', '*');
}


void update_info_win(int life, int score) {
    wmove(info_win, 2, 2);
    wprintw(info_win, "LIFE: %d", life);
    wmove(info_win, 3, 2);
    wprintw(info_win, "SCORE: %d", score);
}

void update_game_win(node* header) {
    node* curr;

    // 아예 game_win을 clear시켜버리고, 각 단어 위치에다가 새로 그린다
    wclear(game_win);
    box(game_win, '*', '*');
    
    for (curr = header->rlink; curr != header; curr = curr->rlink) {
        wmove(game_win, curr->y, curr->x);
        wprintw(game_win, "%s", curr->word);   
    }
}



////////////////////////////////////////////////////////
// 테스트를 위한 파일
// timer.c에 있는 기능 그대로 가져옴
// 일단 word drop만 임의로 해 보자
// 걍 여기서 완성해서 나중에 모듈로 분리하는 게 맞을 듯

#define INTERVAL 100
#define WORD_DROP_C_INIT 700
// #define NEW_WORD_C_INIT 2000

int elapsed_time;
int word_drop_c;
// int new_word_c;






void drop_word(node* header) {
    node* curr;
    for (curr = header->rlink; curr != header; curr = curr->rlink) {
        // word drop 처리 
        curr->y += 1;
        
        // game_win 맨 밑에 닿았다면?
        // TODO
    }
}











void trigger() {
    // 매 trigger마다 변수 갱신
    elapsed_time += INTERVAL;
    word_drop_c -= INTERVAL;
    // new_word_c -= INTERVAL;

    // info_win에 정보 갱신 (LIFE, SCORE)
    update_info_win(100, elapsed_time);  // LIFE는 나중에 매크로로 뺄 것



    // 각 window를 refresh
    wrefresh(game_win);
    wrefresh(info_win);
    wrefresh(typing_win);
    // typing window는 refresh하면 안 되는 것 아닌가?


    if (word_drop_c == 0) {
        // word_drop_c마다 word drop 후 game_win update 필요
        drop_word(list_header);
        update_game_win(list_header);


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

void init_game() {
    list_header = (node*)malloc(sizeof(*list_header));
    list_header->llink = list_header->rlink = list_header;

    ///////////////////// DEBUG!
    node* tmp;

    tmp = get_node("apple", 2, 50);
    insert_node(list_header->llink, tmp);

    tmp = get_node("banana", 4, 20);
    insert_node(list_header->llink, tmp);

    tmp = get_node("cat", 6, 30);
    insert_node(list_header->llink, tmp);

    tmp = get_node("dog", 8, 70);
    insert_node(list_header->llink, tmp);

    

    ///////////////////// DEBUG!
}











void input_handler(node* header, char str[]) {
    node* curr;
    node* temp;
    for (curr = header->rlink; curr != header; curr = curr->rlink) {
        if (!strcmp(curr->word, str)) { 
            // 단어 일치
            printf("found\n");

            // 그 노드 삭제
            temp = curr->llink;
            delete_node(header, curr);
            curr = temp;

            // 삭제 후 화면 갱신 한번 필요
            update_game_win(header);

            return;
        }
    }

    // 일치하는 단어 없음
    // 그대로 아무것도 안 하고 종료
    printf("not found\n");
}











int main() {
    initscr();
    clear();
    refresh();

    prepare_windows();

    char input_str[40];
    int input_len = 0;
    int i;

    init_game();
    init_timer();


    // 시작하자마자 trigger 한번 실행?
    trigger();


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

        // input 받은 단어가 curr_list에 있는지 쭉 돌면서 체크 후,
        // 있으면 노드 삭제하고, 그 위치에서 단어 길이만큼 공백으로 채우고 (노드에 위치정보 있음)
        // 없으면 걍 iterator 끝까지 돌린 것
        // 이후 종료하기 전에 game_win refresh해주고 종료

        // 그럼 game_win 화면 갱신은 언제 일어나는가?

        // 입력 받을때 curr_list iterate하는 함수에서 game_win refresh해 주는 거랑
        // 새 단어 생길 때, word drop할 떄 말고는 game_win refresh 없지 않나?

        input_handler(list_header, input_str);




        
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
