#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <fcntl.h>
#include <termio.h>
#include <unistd.h>
#include "../main_menu.h"

#define SINGLE_MENU_ENDLESS_Y 19
#define SINGLE_MENU_TIMEATTACK_Y 23
#define SINGLE_MENU_BACK_Y 27

void start_single_menu();
int select_single_menu();
void draw_single_menu();

