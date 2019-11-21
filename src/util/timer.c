#include <sys/time.h>
#include <signal.h>
#include "timer.h"

// 1000ms == 1s
// 500ms == 0.5s



// 매 INTERVAL마다 호출됨
void trigger() {
    // trigger 변수들 갱신
    elapsed_time += INTERVAL;
    word_drop_c -= INTERVAL;
    new_word_c -= INTERVAL;
    score_update_c -= INTERVAL;

    // 각각 trigger 조건 확인
    if (word_drop_c == 0) {
        // word drop 필요
        // 단어 맨 밑에 닿는 건 word drop에서 체크해야 함

        // word drop counter 초기값으로 다시 두기
        word_drop_c = WORD_DROP_C_INIT;
    }

    if (new_word_c == 0) {
        // 새 단어 추가 필요

        // new word counter 초기값으로 다시 두기
        new_word_c = NEW_WORD_C_INIT;
    }

    if (score_update_c == 0) {
        // 점수 갱신 필요
        update_score();
        // 초기값으로 다시 두기
        score_update_c = SCORE_UPDATE_C_INIT;
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

// 게임 시작 시 타이머 초기화 (1회 호출되어야 함)
void init_timer() {
    // trigger 변수들 초기화
    elapsed_time = 0;
    word_drop_c = WORD_DROP_C_INIT;
    new_word_c = NEW_WORD_C_INIT;
    score_update_c = SCORE_UPDATE_C_INIT;

    set_ticker(INTERVAL);
    signal(SIGALRM, trigger);
}


