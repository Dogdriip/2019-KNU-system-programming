#include <stdlib.h>
#include <string.h>
#include "game.h"

void init_game() {
    // 현재 화면에 나와 있는 단어 리스트 초기화
    curr_list = NULL;  // first

}

word_node* get_node() {
    // get_word() 호출해서 나온 랜덤 단어로 노드 만들어서 반환함
    word_node* tmp = (word_node*)malloc(sizeof(*tmp));
    
    // 랜덤 단어
    char *random_word = get_word(1, 12);
    strcpy(random_word, tmp->word);

    // y는 맨 위로 고정, x는 랜덤
    tmp->y = 0;
    tmp->x = 0;

    tmp->link = NULL;

    return tmp;
}

void insert_node(word_node* head, word_node* node) {
    node->next = head->next;
    head->next = node;
}

