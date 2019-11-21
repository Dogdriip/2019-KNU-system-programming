#pragma once

// definitions
#define SCORE_UPDATE_VAL 1000  // score_update_c마다 점수 얼마나 증가시킬 것인가?

typedef struct word_node* word_nodeptr;
typedef struct word_node {
    char* word;  // word content
    int y, x;  // pos (row, col)
    word_nodeptr link;
} word_node;


// variables
int game_score;  // 게임 중 현재 점수. 매 score_update_c마다 갱신됨
int game_min_len, game_max_len;  // word len limits during game
word_nodeptr curr_list;


// functions
void init_game();
void update_score();
word_nodeptr get_node();
// void del_node(word_nodeptr*, word_nodeptr, word_nodeptr);
// void insert_node(word_nodeptr*, word_nodeptr, int);