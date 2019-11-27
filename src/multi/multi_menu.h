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

#define MULTI_MENU_MATCH_Y 19
#define MULTI_MENU_BACK_Y 27

int flag_multi_connection = 0;

int multi_connection();
void start_multi_menu();
int select_multi_menu();
void draw_multi_menu();

