#include <stdlib.h>
#include <string.h>
#include "game.h"

void init_game() {
    // 점수 변수 초기화
    game_score = 0;

    // 현재 화면에 나와 있는 단어 리스트 초기화
    curr_list = NULL;  // first

}

void update_score() {
    // SCORE_UPDATE_VAL만큼 점수 증가
    game_score += SCORE_UPDATE_VAL;
}

word_nodeptr get_node() {
    // get_word() 호출해서 나온 랜덤 단어로 노드 만들어서 반환함
    word_nodeptr tmp = (word_nodeptr)malloc(sizeof(*tmp));
    
    // 랜덤 단어
    strcpy(get_word(1, 12), tmp->word);

    // y는 맨 위로 고정, x는 랜덤
    tmp->y = 0;
    tmp->x = 0;

    tmp->link = NULL;

    return tmp;
}

