#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <fcntl.h>
#include <termio.h>
#include <unistd.h>
#include "../main_menu.h"

#define MULTI_MENU_MATCH_Y 19
#define MULTI_MENU_BACK_Y 27

void start_multi_menu();
int select_multi_menu();
void draw_multi_menu();

