#pragma once

// definitions
// 점수는 timer.c의 elapsed_time으로 대체됨

typedef struct word_node {
    char* word;  // word content
    int y, x;  // pos (row, col)
    word_node* link;
} word_node;


// variables
int game_min_len, game_max_len;  // word len limits during game
word_node* curr_list;


// functions
void init_game();
word_node* get_node();
void insert_node(word_node*, word_node*);
// void del_node(word_nodeptr*, word_nodeptr, word_nodeptr);
