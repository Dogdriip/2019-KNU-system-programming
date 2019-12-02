#include "multi_game.h"

//////////////////////////////////////////////////////////////
// linked list 관련
node* list_header;  // 현재 게임판에 나와있는 단어들 리스트
node* multi_get_node(char word[], int y, int x) {
    node* tmp = (node*)malloc(sizeof(*tmp));
    
    strcpy(tmp->word, word);
    tmp->y = y; tmp->x = x; tmp->llink = NULL; tmp->rlink = NULL;

    return tmp;
}

void multi_insert_node(node* destnode, node* newnode) {
	newnode->llink = destnode;
	newnode->rlink = destnode->rlink;
	destnode->rlink->llink = newnode;
	destnode->rlink = newnode;
}

void multi_delete_node(node* destnode, node* deleted) {
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
WINDOW* game_win;   // 좌상단 유저 게임 화면 출력창
WINDOW* other_win;  // 우상단 상대 게임 화면 출력창
WINDOW* typing_win;  // 하단 유저 입력창

int remain_life;
char screen_edge[MULTI_GAME_WIN_WIDTH + 1];

void multi_prepare_windows() {
    game_win = newwin(MULTI_GAME_WIN_HEIGHT, MULTI_GAME_WIN_WIDTH, MULTI_GAME_WIN_Y, MULTI_GAME_WIN_X);
    box(game_win, '*', '*');

   	other_win = newwin(MULTI_OTHER_WIN_HEIGHT, MULTI_OTHER_WIN_WIDTH, MULTI_OTHER_WIN_Y, MULTI_OTHER_WIN_X);
    box(other_win, '*', '*');

    typing_win = newwin(TYPING_WIN_HEIGHT, TYPING_WIN_WIDTH, TYPING_WIN_Y, TYPING_WIN_X);
    box(typing_win, '*', '*');

	for(int i = 0; i < MULTI_GAME_WIN_WIDTH; i++)
		screen_edge[i] = '*';
	mvwprintw(game_win, 2, 0, "%s", screen_edge);
	mvwprintw(other_win, 2, 0, "%s", screen_edge);
	wrefresh(other_win);
}

void multi_update_game_win(node* header) {
    node* curr;

    // 아예 game_win을 clear시켜버리고, 각 단어 위치에다가 새로 그린다
    wclear(game_win);
    box(game_win, '*', '*');
	wmove(game_win, 2, 0);
	wprintw(game_win, "%s", screen_edge);

	wmove(game_win, 1, 2);
	wprintw(game_win, "HP : %d", remain_life);
  
    for (curr = header->rlink; curr != header; curr = curr->rlink) {
        wmove(game_win, curr->y, curr->x);
        wprintw(game_win, "%s", curr->word);   
    }

	wrefresh(game_win);
}
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// 게임플레이
int elapsed_time;
int word_drop_c;
int new_word_c;

int communication_c;
int multi_fd;
int flag_multi_game = 1;
pthread_mutex_t mutex_lock;

int multi_set_ticker(int n_msecs) {
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

void multi_gameover(){
	if (flag_multi_game == 2)
		return;
	flag_multi_game = -1;

    multi_set_ticker(0);
	signal(SIGALRM, SIG_IGN);

	write(multi_fd, &flag_multi_game, sizeof(int));
	close(multi_fd);

}
void multi_game_win(){
	if (flag_multi_game == -1)
		return;
	flag_multi_game = 2;

	multi_set_ticker(0);
	signal(SIGALRM, SIG_IGN);

	close(multi_fd);
}
void multi_drop_word(node* header) {
    node* curr;
    node* temp;
    for (curr = header->rlink; curr != header; curr = curr->rlink) {
        // word drop 처리 
        curr->y += 1;
        
        // game_win 맨 밑에 닿았다면?
        if (curr->y >= MULTI_GAME_WIN_HEIGHT - 1) {
            // 그 단어 삭제 후 화면 갱신
            temp = curr->llink;
            multi_delete_node(header, curr);
            curr = temp;

            multi_update_game_win(header);
            
            // life 1 감소
            remain_life -= 1;
            if (remain_life <= 0) {
                multi_gameover();
            }
            // update_info_win(remain_life)
        }
    }
}

void multi_add_new_word(node* header) {
    node* tmp;
    char word[MAX_STRING_LENGTH + 1];

    strcpy(word, get_word(MIN_STRING_LENGTH, MAX_STRING_LENGTH));
    
    tmp = multi_get_node(word, 3, (rand() % (MULTI_GAME_WIN_WIDTH - strlen(word) - 2)) + MULTI_GAME_WIN_X + 1);
	
	multi_insert_node(list_header->llink, tmp);
}

void multi_input_handler(node *header, char str[]){
	node* curr;
	node* temp;
	for (curr = header->rlink; curr != header; curr = curr->rlink){
		if (!strcmp(curr->word, str)){

			temp = curr->llink;
			multi_delete_node(header, curr);
			curr = temp;

            //tructive 삭제 후 화면 갱신 한번 필요
            multi_update_game_win(header);

            return;
        }
    }
}

void* multi_communication(void* m){
	char message[1000000];

	FILE *fp_screen;
	int total_len = 0, len = 0, other_flag = 0;
    // typing window는 refresh하면 안 되는 것 아닌가?

	pthread_mutex_lock(&mutex_lock);
	if (flag_multi_game == -1 || flag_multi_game == 2)
		return NULL;
	fp_screen = fopen("cache_screen.bin", "wb+");
	write(multi_fd, &flag_multi_game, sizeof(int));
	read(multi_fd, &other_flag, sizeof(int));
	if (other_flag == -1){
		flag_multi_game= 2;
		fclose(fp_screen);
		pthread_mutex_unlock(&mutex_lock);
		multi_game_win();
		return NULL;
	}

	putwin(game_win, fp_screen);
	fseek(fp_screen, 0, SEEK_SET);

	total_len = fread(message, sizeof(char), 100000, fp_screen); // 파일로부터 데이터를 읽어옴
	while(!feof(fp_screen))
		total_len += fread(&message[total_len], sizeof(char), 100000, fp_screen);

	write(multi_fd, &total_len, sizeof(int)); // 서버에 총길이를 보냄
	write(multi_fd, message, sizeof(char) * total_len); // 서버로 게임 화면을 보냄
	fclose(fp_screen);

	read(multi_fd, &total_len, sizeof(int)); // 서버로부터 총 길이를 받음
	len = read(multi_fd, message, sizeof(char) * total_len); // 서버로부터 화면을 받음
	while(len < total_len) 
		len += read(multi_fd, &message[len], sizeof(char) * (total_len - len));
	pthread_mutex_unlock(&mutex_lock);
	
	fp_screen = fopen("cache_screen2.bin", "wb+");;
	fwrite(message, sizeof(char) * total_len, 1, fp_screen);
	fflush(fp_screen);
	fseek(fp_screen, 0, SEEK_SET);

	wclear(other_win);
	wrefresh(other_win);
	mvwin(other_win, MULTI_GAME_WIN_Y, MULTI_GAME_WIN_X);
	overwrite(getwin(fp_screen), other_win);
	mvwin(other_win, MULTI_OTHER_WIN_Y, MULTI_OTHER_WIN_X);
	wrefresh(other_win);
	fclose(fp_screen);
}

void multi_trigger() {
    // 매 trigger마다 변수 갱신
    elapsed_time += CLOCK_INTERVAL;
    word_drop_c -= CLOCK_INTERVAL;
    new_word_c -= CLOCK_INTERVAL;
	communication_c -= CLOCK_INTERVAL;

    // 각 window를 refresh
    wrefresh(game_win);
    wrefresh(typing_win);

    if (word_drop_c == 0) {
        // word_drop_c마다 word drop 후 game_win update 필요
        multi_drop_word(list_header);
        multi_update_game_win(list_header);

        word_drop_c = WORD_DROP_C_INIT;
    }

    if (new_word_c == 0) {
        // 새 단어 추가 필요
        multi_add_new_word(list_header);
        multi_update_game_win(list_header);

        new_word_c = NEW_WORD_C_INIT;
    }

	if (communication_c == 0){
		// 스크린 송수신

		pthread_t t1;
		pthread_create(&t1, NULL, multi_communication, NULL);
		pthread_detach(t1);

		communication_c = MULTI_INIT_COMMUNICATION_C;

	}

}

void multi_init_timer() {
    elapsed_time = 0;
    word_drop_c = WORD_DROP_C_INIT;
    new_word_c = NEW_WORD_C_INIT;
	communication_c = MULTI_INIT_COMMUNICATION_C;

    multi_set_ticker(CLOCK_INTERVAL);
    signal(SIGALRM, multi_trigger);
}

void multi_init_game() {
	flag_multi_game = 1;
    remain_life = LIFE_INIT;
	remain_life = 1;

    list_header = (node*)malloc(sizeof(*list_header));
    list_header->llink = list_header->rlink = list_header;
}

/////////
// 입력을 스레드로 빼는게 깔끔할 듯 

void* input_func(void* thr_data){
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

		multi_input_handler(list_header, input_str);
	}
}

/////////

//////////////////////////////////////////////////////////////
// main game logic
void start_multi_game(int fd) {
    int i;
	multi_fd = fd;

    clear(); refresh();
    echo();

    multi_prepare_windows();
	pthread_mutex_init(&mutex_lock, NULL);

    multi_init_game();
    multi_init_timer();

	pthread_t thr_input;
	pthread_create(&thr_input, NULL, input_func, NULL);

	while(flag_multi_game == 1)
		sleep(1);

	pthread_cancel(thr_input);
    
    wclear(game_win);
    wclear(other_win);
    wclear(typing_win);

	wrefresh(game_win);
	wrefresh(other_win);
	wrefresh(typing_win);

    delwin(game_win);
    delwin(other_win);
    delwin(typing_win);

	WINDOW *gameover_win = newwin(LINES - 10, COLS - 10, 5, 5);
	box(gameover_win, '*', '*');
	mvwprintw(gameover_win, (LINES - 10) / 2 + 5, (COLS - 10 - strlen("GAMEOVER!!")) / 2, "GAMEOVER!!");
	if (flag_multi_game == 2)
		mvwprintw(gameover_win, (LINES - 10) / 2 + 7, (COLS - 10 - strlen("Win")) / 2, "Win");
	else
		mvwprintw(gameover_win, (LINES - 10) / 2 + 7, (COLS - 10 - strlen("Lose")) / 2, "Lose");
	wrefresh(gameover_win);

	if (MENU_TITLE_Y + 10 < (LINES - 10) / 2 + 5)
		draw_title();
	sleep(1);
	
	mvwprintw(gameover_win, (LINES - 10) / 2 + 10, (COLS - 10 - strlen("Please Enter Key...")) / 2, "Please Enter Key...");
	wrefresh(gameover_win);

	while(getch() != '\n');

	wclear(gameover_win);
	wrefresh(gameover_win);
	delwin(gameover_win);
}
