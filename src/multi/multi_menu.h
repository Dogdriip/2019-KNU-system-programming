#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <pthread.h>
#include "../util/connection.h"
#include "../main_menu.h"
#include "multi_game.h"

#define MY_LINES 41
#define MY_COLS 181

#define MULTI_MENU_MATCH_Y (LINES / 2)
#define MULTI_MENU_BACK_Y (LINES - 10)

typedef struct multi_info{
	int flag;
	int fd;
	char message[30];
}multi_info;

multi_info connecting();
void start_multi_menu();
int select_multi_menu();
void draw_multi_menu();

