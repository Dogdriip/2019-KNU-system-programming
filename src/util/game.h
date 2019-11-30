/*
 * 싱글, 멀티 전체적으로 적용되는 함수들, 상수들
 */

// 스크린 상하좌우 여백
#define GAME_SCR_PADDING 5


// 게임플레이 관련
#define LIFE_INIT 5
#define CLOCK_INTERVAL 100
#define WORD_DROP_C_INIT 300
#define NEW_WORD_C_INIT 500


// doubly linked list
typedef struct node {
    char word[40];  // word content
    int y, x;  // pos (row, col)
    struct node* llink;
    struct node* rlink;
} node;

node* get_node(char[], int, int);
void insert_node(node*, node*);
void delete_node(node*, node*);
