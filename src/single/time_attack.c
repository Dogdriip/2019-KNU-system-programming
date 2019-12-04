#include "time_attack.h"

// ta_



//////////////////////////////////////////////////////////////
// linked list 관련
node* list_header;  // 현재 게임판에 나와있는 단어들 리스트

node* ta_get_node(char word[], int y, int x) {
    node* tmp = (node*)malloc(sizeof(*tmp));
    
    strcpy(tmp->word, word);
    tmp->y = y; tmp->x = x; tmp->llink = NULL; tmp->rlink = NULL;

    return tmp;
}

void ta_insert_node(node* destnode, node* newnode) {
	newnode->llink = destnode;
	newnode->rlink = destnode->rlink;
	destnode->rlink->llink = newnode;
	destnode->rlink = newnode;
}

void ta_delete_node(node* destnode, node* deleted) {
	if (destnode == deleted) {
		printf("Deletion of header node not permitted.\n");
	} else {
		deleted->llink->rlink = deleted->rlink;
		deleted->rlink->llink = deleted->llink;
		free(deleted);
	}
}
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// window 관련
WINDOW* game_win;  // 좌상단 가장 큰 게임 메인창
WINDOW* info_win;  // 우상단 현재 게임 정보 출력창
WINDOW* typing_win;  // 하단 유저 입력창

WINDOW* gameover_win;  // 게임오버 창

void ta_prepare_windows() {
    game_win = newwin(GAME_WIN_HEIGHT, GAME_WIN_WIDTH, GAME_WIN_Y, GAME_WIN_X);
    box(game_win, '*', '*');

    info_win = newwin(INFO_WIN_HEIGHT, INFO_WIN_WIDTH, INFO_WIN_Y, INFO_WIN_X);
    box(info_win, '*', '*');

    typing_win = newwin(TYPING_WIN_HEIGHT, TYPING_WIN_WIDTH, TYPING_WIN_Y, TYPING_WIN_X);
    box(typing_win, '*', '*');
}

void ta_update_info_win(int remain_time, int score) {
    wmove(info_win, 4, 6);
    wprintw(info_win, "REMAIN TIME: %.1lf", (double)remain_time / 1000);
    wmove(info_win, 6, 6);
    wprintw(info_win, "SCORE: %d", score);
}

void ta_update_game_win(node* header) {
    node* curr;

    // 아예 game_win을 clear시켜버리고, 각 단어 위치에다가 새로 그린다
    wclear(game_win);
    box(game_win, '*', '*');
    
    for (curr = header->rlink; curr != header; curr = curr->rlink) {
        wmove(game_win, curr->y, curr->x);
        wprintw(game_win, "%s", curr->word);   
    }
}
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// 게임플레이
short int FLAG;  // 게임 종료 여부
int elapsed_time;
int word_drop_c;
int new_word_c;
int score;  // 처리한 단어 수

// 난이도에 따라 값이 바뀌어야 하므로 변수에 저장
int word_drop_c_interval;
int new_word_c_interval;

void ta_gameover() {
    signal(SIGALRM, SIG_IGN);
	FLAG = 0;
    

}

void ta_drop_word(node* header) {
    node* curr;
    node* temp;
    for (curr = header->rlink; curr != header; curr = curr->rlink) {
        // word drop 처리 
        curr->y += 1;
        
        // game_win 맨 밑에 닿았다면?
        if (curr->y >= GAME_WIN_HEIGHT - 1) {
            // 그 단어 삭제 후 화면 갱신
            temp = curr->llink;
            ta_delete_node(header, curr);
            curr = temp;

            ta_update_game_win(header);
        }
    }
}

void ta_add_new_word(node* header) {
    node* tmp;
    char word[MAX_STRING_LENGTH + 1];

    strcpy(word, get_word(MIN_STRING_LENGTH, MAX_STRING_LENGTH));
    
    tmp = ta_get_node(word, 2, (rand() % (GAME_WIN_WIDTH - MAX_STRING_LENGTH - 3) + GAME_WIN_X + 1));
    ta_insert_node(list_header->llink, tmp);
}

void ta_input_handler(node* header, char str[]) {
    node* curr;
    node* temp;
    for (curr = header->rlink; curr != header; curr = curr->rlink) {
        if (!strcmp(curr->word, str)) { 
            // 단어 일치
            // 그 노드 삭제
            temp = curr->llink;
            ta_delete_node(header, curr);
            curr = temp;

            // 삭제 후 화면 갱신 한번 필요
            ta_update_game_win(header);

            // 점수 증가
            score++;

            return;
        }
    }

    // 일치하는 단어 없음
    // 그대로 아무것도 안 하고 종료
}

int ta_set_ticker(int n_msecs) {
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

void ta_trigger() {
    // 매 trigger마다 변수 갱신
    elapsed_time += CLOCK_INTERVAL;
    word_drop_c -= CLOCK_INTERVAL;
    new_word_c -= CLOCK_INTERVAL;

    // info_win에 정보 갱신 (LIFE, SCORE)
    ta_update_info_win(TIME_ATTACK_TIME_INIT - elapsed_time, score);

    // 각 window를 refresh
    wrefresh(game_win);
    wrefresh(info_win);
    wrefresh(typing_win);
    // typing window는 refresh하면 안 되는 것 아닌가?

    if (word_drop_c <= 0) {
        // word_drop_c마다 word drop 후 game_win update 필요
        ta_drop_word(list_header);
        ta_update_game_win(list_header);

        word_drop_c = word_drop_c_interval;
    }

    if (new_word_c <= 0) {
        // 새 단어 추가 필요
        ta_add_new_word(list_header);
        ta_update_game_win(list_header);

        new_word_c = new_word_c_interval;
    }

    if (TIME_ATTACK_TIME_INIT - elapsed_time <= 0) {
        ta_gameover();
    }
}

void ta_init_timer() {
    elapsed_time = 0;
    word_drop_c = word_drop_c_interval = TA_WORD_DROP_C_INIT;
    new_word_c = new_word_c_interval = TA_NEW_WORD_C_INIT;

    ta_set_ticker(CLOCK_INTERVAL);
    signal(SIGALRM, ta_trigger);
}

void ta_init_game() {
    FLAG = 1;

    list_header = (node*)malloc(sizeof(*list_header));
    list_header->llink = list_header->rlink = list_header;
}
//////////////////////////////////////////////////////////////
void* ta_input_func(void *thr_data){
	char input_str[40];
	int input_len = 0;
	int i;

	while(1){
		wmove(typing_win, 2, 2);
		wgetstr(typing_win, input_str);
		input_len = strlen(input_str);
	
		// input 받았으면 공백으로 채우자
		for (i = 2; i < input_len + 2; i++) {
			wmove(typing_win, 2, i);
			wprintw(typing_win, " ");
		}

		ta_input_handler(list_header, input_str);
	}
}
//////////////////////////////////////////////////////////////
// main game logic
int single_time_attack_game() {
    int i;

    echo();
    clear(); refresh();

    ta_prepare_windows();

    ta_init_game();
    ta_init_timer();

	pthread_t thr_input;
	pthread_create(&thr_input, NULL, ta_input_func, NULL);
	
    while (FLAG) 
		sleep(1);

	pthread_cancel(thr_input);

    wclear(game_win);
    wclear(info_win);
    wclear(typing_win);

    wrefresh(game_win);
    wrefresh(info_win);
    wrefresh(typing_win);

	delwin(game_win);
	delwin(typing_win);
	delwin(info_win);

    gameover_win = newwin(GAMEOVER_WIN_HEIGHT, GAMEOVER_WIN_WIDTH, GAMEOVER_WIN_Y, GAMEOVER_WIN_X);
    box(gameover_win, '*', '*');

    char gameover_str[] = "GAME OVER";
    wmove(gameover_win, GAMEOVER_WIN_Y + 5, (COLS - 10 - strlen(gameover_str)) / 2);
    wprintw(gameover_win, gameover_str);

    wmove(gameover_win, GAMEOVER_WIN_Y + 8, (COLS - 10 - strlen("SCORE :    ")) / 2);
    wprintw(gameover_win, "SCORE : %3d", score);

    wrefresh(gameover_win);

    sleep(1);
	
	mvwprintw(gameover_win, (LINES - 10) / 2 + 10, (COLS - 10 - strlen("Please Enter Key...")) / 2, "Please Enter Key...");
	wrefresh(gameover_win);

	while(getch() != '\n');

	return score;
}
