/***main.h***/
#ifndef MAIN_H
#define MAIN_H


#include "snake.h"

#define MENU_ROWS_ADD	3
#define MENU_COLS_ADD	4

#define MENU_STARTY	5
#define MENU_STARTX	2*MENU_STARTY


#define CREDITS_FILE	"credits.txt"
#define GAME_NAME   	"had"
#define BUFF_MAX		50
#define STATUS_TEXT_LEN	200
#define LIFE_DOWN_DELAY_US	500000

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

