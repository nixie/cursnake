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

SEGMENT *init_fruits(int how_many, COORDS max);
void free_fruits(SEGMENT *fruits);
int fruits_count(SEGMENT *fruits);	
int remove_fruit( SEGMENT *p_fruits, COORDS xy);
void fruits_to_map( SEGMENT *fruits, int **map, int x, int y);
void snake_to_map( SNAKE *snake, int **map, int x, int y);	
void clear_map(int **map, int x, int y);
CONTENTS map_ok(int **map, int curr_x, int curr_y,
	   					   int max_x, int max_y,
					   	   NSEW direction);
void render_map( int **p_p_from, WINDOW *p_to, int sizex, int sizey);

// helper for get_closest_fruit()
int	measure_lenght(COORDS *from, COORDS *to);

COORDS get_closest_fruit(SEGMENT *fruits, COORDS *curr_pos);
void ai_set_dirr(COORDS *to, SNAKE *snake);

// returns nonzero if dir1 is antagonic (NORTH, SOUTH or WEST, EAST)
int is_antagonic(int dir1, int dir2);

#endif
