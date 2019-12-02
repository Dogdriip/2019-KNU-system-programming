#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "../main_menu.h"
#include "endless.h"
#include "time_attack.h"
#include "single_gameover.h"

#define SINGLE_MENU_ENDLESS_Y (LINES / 2)
#define SINGLE_MENU_TIMEATTACK_Y (LINES / 2 + 4)
#define SINGLE_MENU_BACK_Y (LINES / 2 + 8)

void start_single_menu();
int select_single_menu();
void draw_single_menu();

