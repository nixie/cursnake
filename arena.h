/***arena.h***/
#ifndef ARENA_H
#define ARENA_H

#include "snake.h"

#define INIT_FRUITS 10
#define BEGINNER_SPEED	200		// every move hapens in miliseconds
#define ADVANCED_SPEED	100
#define PROFI_SPEED		50

#define SCORE_INCREMENT	10
#define BODY_INCREMENT	5

#define CHAR_FREE	' '
#define CHAR_FRUIT	'$'
#define CHAR_WALL	'#'


typedef enum {FREE, WALL, PL1_BODY, PL1_HEAD, PL2_BODY, PL2_HEAD, FRUIT} CONTENTS;

int ** alloc_map(int rows, int row_width);
int dealloc_map( int **p_p_map, int rows);

SEGMENT *init_fruits(int how_many, COORDS max);//x
void free_fruits(SEGMENT *fruits);//xxx
int fruits_count(SEGMENT *fruits);	//xxx
int remove_fruit( SEGMENT *p_fruits, COORDS xy);//xxx
void fruits_to_map( SEGMENT *fruits, int **map, int x, int y); //xxx
void snake_to_map( SNAKE *snake, int **map, int x, int y);	//xxx
void clear_map(int **map, int x, int y);	//xxx
CONTENTS map_ok(int **map, int curr_x, int curr_y,	//xxx
	   					   int max_x, int max_y,
					   	   NSEW direction);
void render_map( int **p_p_from, WINDOW *p_to, int sizex, int sizey);

// returns nonzero if dir1 is antagonic (NORTH, SOUTH or WEST, EAST)
int is_antagonic(int dir1, int dir2);

#endif
