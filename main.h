/*
This file is part of game called CURSNAKE.

Cursnake is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Cursnake is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Cursnake.  If not, see <http://www.gnu.org/licenses/>.
*/

/***main.h***/
#ifndef MAIN_H
#define MAIN_H

#define MENU_ROWS_ADD	3
#define MENU_COLS_ADD	4

#define MENU_STARTY	5
#define MENU_STARTX	2*MENU_STARTY


#define CREDITS_FILE	"credits.txt"
#define GAME_NAME   	"had"
#define BUFF_MAX		50
#define STATUS_TEXT_LEN	200
#define LIFE_DOWN_DELAY_US	500000


// every move hapens in *_SPEED miliseconds
#define BEGINNER_SPEED	100 // -s 10	
#define ADVANCED_SPEED	40  // -s 25
#define PROFI_SPEED		25  // -s 40

#define INIT_FRUITS 10

#define SCORE_INCREMENT	10
#define BODY_INCREMENT	5


// this enums must be in tact with menu items from main.c
typedef enum{PLAY=0, GAMESTYLE, DIFFICULTY, PLAYER_SETUP, CREDITS, EXIT} E_MENU_ITEMS;	
typedef enum{BEGINNER=0, ADVANCED, PROFI} E_DIFFICULTY;
typedef enum{ONE=0, TWO} E_PLAYERS;
typedef enum{CLASSIC=0, NOWALLS, FISHES, TRON} E_GAMESTYLE;	// fishes == fruit collecting

typedef struct keypair{
	int key1;
	int key2;
} KEYPAIR_T;

int show_menu(char **contents);

void init_defaults(void);
int start_game(void);
void  flush_input(void);

void wait_for_key(int how_long);

void rprint_credits(WINDOW *win);
int set_cli_opts(int argc, char *argv[]);

// adds string or integer to status line



// converts keycode(KEY_UP, KEY_LEFT...) to NORTH, SOUTH...
NSEW key1_to_NSEW(int key);
int NSEW_to_key1(NSEW dir);
NSEW key2_to_NSEW(int key);
int NSEW_to_key2(NSEW dir);
#endif

