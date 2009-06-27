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

// init linked list od fruits and randomize their position
SEGMENT *init_fruits(int how_many, COORDS max);

// dealloc memory
void free_fruits(SEGMENT *fruits);

// this function returns number of fruits
int fruits_count(SEGMENT *fruits);	

// remove fruit identified by its position
void remove_fruit( SEGMENT *p_fruits, COORDS xy);

// render fruits to map
void fruits_to_map( SEGMENT *fruits, int **map, int x, int y);

// for debugging purposes
void print_fruit_coords(SEGMENT *p_fruits, WINDOW *win, COORDS coord);

// render snake
void snake_to_map( SNAKE *snake, int **map, int x, int y);	

// fill **map with FREE (space)
void clear_map(int **map, int x, int y);

// checkes what is on place, we want to go
CONTENTS map_ok(int **map, int curr_x, int curr_y,
	   					   int max_x, int max_y,
					   	   NSEW direction);

// draw map CONTENTS to WINDOW.
void render_map( int **p_p_from, WINDOW *p_to, int sizex, int sizey);

// returns nonzero if dir1 is antagonic (NORTH, SOUTH or WEST, EAST)
int is_antagonic(int dir1, int dir2);



#endif
