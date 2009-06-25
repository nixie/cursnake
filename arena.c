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


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <curses.h>

#include "common.h"
#include "snake.h"
#include "main.h"
#include "arena.h"

int **alloc_map( int rows, int row_width){
	// first alloc pointers to rows
	int r,d;

	int **p_tmp = (int **)malloc( rows * sizeof(int *));
	if (p_tmp == NULL){
		return NULL;
	}

	for (r=0; r<rows; r++){
		p_tmp[r] = (int *)malloc( row_width * sizeof(int));
		if (p_tmp[r] == NULL){
			for (d=0; d<r; ++d){
				free( (void *)p_tmp[r]);
			}	
			return NULL;
		}
	}
	return p_tmp;
}

int dealloc_map(int **p_p_map, int rows){
	int r;

	for (r=0; r<rows; ++r){
		free((void*) p_p_map[r]);
	}
	free( (void *) p_p_map);
	return 0;
}


SEGMENT *init_fruits( int how_many, COORDS max){
	// generate linked list
	SEGMENT *p_first = (SEGMENT *)malloc( sizeof(SEGMENT) );
	SEGMENT *p_tmp = p_first;
	p_first->position.x = -1;	// we should not to delete this memory.(eg let snake to eat it)
	p_first->position.y = -1;	// this pointer is gate to our list

	int i;
	for	(i=0; i<how_many; i++){
		p_tmp->p_next = (SEGMENT *)malloc( sizeof(SEGMENT) );
		p_tmp =p_tmp->p_next;
	}
	p_tmp->p_next = NULL;

	// generate random places for fruits
	p_tmp = p_first->p_next;


	while (p_tmp != NULL){
		// set coords		
		p_tmp->position.x = rand() % (max.x-1);
		p_tmp->position.y = rand() % (max.y-1);
		// go to next fruit
		p_tmp = p_tmp->p_next;
	}
	

	return p_first;
}

void free_fruits( SEGMENT *fruits){

	SEGMENT *p_akt=fruits;
	SEGMENT *p_nex;

	while(p_akt != NULL){
		p_nex = p_akt->p_next;
		free( (void *)p_akt);
		p_akt = p_nex;
	}
}//free_fruits()



int fruits_count(SEGMENT *p_fruits){
	// first item in linked list fruits is not actually fruit.
	SEGMENT *p_tmp= p_fruits->p_next;
	int i;
	for( i =0; p_tmp != NULL; p_tmp = p_tmp->p_next){
		i++;
	}
	return i;
}

void remove_fruit( SEGMENT *p_fruits, COORDS xy){
	
	//mvprintw(0,0, "first cycle - removing fruits");
	//sleep(1);
	SEGMENT *p_to_delete;
	SEGMENT *p_parent= p_fruits;

	while(p_parent->p_next != NULL){
		//mvprintw(0,0, "first cycle - removing fruits");
		//sleep(1);
		if ( xy.x == p_parent->p_next->position.x && xy.y == p_parent->p_next->position.y){
			p_to_delete = p_parent->p_next;
			p_parent->p_next = p_parent->p_next->p_next;
			free( (void *)p_to_delete);
			break;
		}
		p_parent = p_parent->p_next;
	}
	return;
}


// helper for get_closest_fruit()
int	measure_lenght(COORDS *from, COORDS *to){
	int len;

	len = abs(from->x - to->x);
	len += abs(from->y - to->y);
	return len;
}

COORDS get_closest_fruit(SEGMENT *fruits, COORDS *curr_pos){
	SEGMENT *actual_fruit = fruits->p_next;	// first item isn't a fruit
	
	COORDS ret_null = {0,0};
	if (actual_fruit == NULL){
		return ret_null;
	}

	// init variables
	COORDS closest_fruit_coords = actual_fruit->position;
	int tmp_lenght; 
	int min_path_lenght = measure_lenght(curr_pos, &(actual_fruit->position)) ;

	actual_fruit = actual_fruit->p_next;
	while (actual_fruit != NULL){
		
		tmp_lenght = measure_lenght(curr_pos, &(actual_fruit->position));
		if (tmp_lenght < min_path_lenght){
			closest_fruit_coords = actual_fruit->position;
			min_path_lenght = tmp_lenght;
		}		

		actual_fruit = actual_fruit->p_next;
	}
	
	return closest_fruit_coords;
}

void print_fruit_coords(SEGMENT *p_fruits, WINDOW *win, COORDS coord){
	SEGMENT *p_s = p_fruits->p_next;
	int r = 3;
	int c = 30;

	while(p_s != NULL){
		mvwprintw(win, r, c, "f%d[%03d,%03d]->%d",
				r-2,
				p_s->position.x,
				p_s->position.y,
				measure_lenght(&coord, &(p_s->position)));
		r++;	
		p_s = p_s->p_next;
	}
	mvwprintw(win, r+1,c+3, "tot:%d", r-3);
}
	


void render_map( int **p_p_from, WINDOW *p_to, int sizex, int sizey){
	int row_to,col_to;
	int x_from=0, 
		y_from=0;

	for (row_to = 1; row_to<sizey+1; row_to++){	
		x_from=0;						
		for (col_to=1; col_to<sizex+1; col_to++, x_from++){
			//TODO:colors (addch(ch | A_BOLD | ...));
			wattrset(p_to, COLOR_PAIR(1) | A_NORMAL);
			switch(p_p_from[y_from][x_from]){
				case FREE:		mvwprintw(p_to, row_to, col_to, "%c", CHAR_FREE);	// print free char
								break;

				case FRUIT:	wattrset(p_to, COLOR_PAIR(3) | A_BOLD);
								mvwaddch(p_to, row_to, col_to, CHAR_FRUIT);
								break;

				case WALL:		mvwaddch(p_to, row_to, col_to, CHAR_WALL);
								break;

				case PL1_BODY:	wattrset(p_to, COLOR_PAIR(1));
								mvwaddch(p_to, row_to, col_to, BODY_MATERIAL);
								break;

				case PL1_HEAD:	wattrset(p_to, COLOR_PAIR(1) | A_BOLD);
								mvwaddch(p_to, row_to, col_to, HEAD_MATERIAL);
								break;

				case PL2_BODY:	wattrset(p_to, COLOR_PAIR(2) );
								mvwaddch(p_to, row_to, col_to, BODY_MATERIAL);
								break;
				case PL2_HEAD:	wattrset(p_to, COLOR_PAIR(2) | A_BOLD);
								mvwaddch(p_to, row_to, col_to, HEAD_MATERIAL);
								break;

				default:	mvwaddch(p_to, row_to, col_to, 'E');
							break;
			}			
		}
		y_from++;
	}
}


void clear_map(int **map, int x, int y){
	int r,c;
	for (r=0; r<y; r++){
		for (c=0; c<x; c++){
			map[r][c] = FREE;
		}
	}
}



void fruits_to_map( SEGMENT *p_fruits, int **map, int x, int y){
	// go through list of fruits
	// check if fruit.position < [x,y]
	// && map[fruit.posi.y][fruit.posi.x] = FRUIT
	
	SEGMENT *p_tmp;
	for (p_tmp = p_fruits->p_next; p_tmp != NULL; p_tmp = p_tmp->p_next){
		if ( p_tmp->position.x < x && p_tmp->position.y <y){
			map[p_tmp->position.y][p_tmp->position.x] = FRUIT;
		}
		else{
			dputs("fruit_to_map() : fruit out of borders!");
		}
	}
}

void snake_to_map( SNAKE *p_snake, int **map, int x, int y){
	// principially same as fruits_to_map()
		SEGMENT *p_tmp;
		int count=0;
	for (p_tmp = p_snake->p_head; p_tmp != NULL; p_tmp = p_tmp->p_next){
		if ( p_tmp->position.x < x && p_tmp->position.y <y){
			if (count ==0){	// first cycle -> this is head
				map[p_tmp->position.y][p_tmp->position.x] = (int)p_snake->map_head;
			}else{
				map[p_tmp->position.y][p_tmp->position.x] = (int)p_snake->map_body;
			}
			count++;		
		}
		else{
			dputs("snake_to_map() : snake out of borders!");
		}
	}
}
/*CONTENTS char_to_contents(int c){
	switch(c){
		case CHAR_FREE:
			return FREE;
			break;

		case CHAR_FRUIT:
			return FRUIT;
			break;

		case CHAR_WALL:
			return WALL;
			break;

		case BODY_MATERIAL:
			return SNAKE_BODY;
			break;

		case HEAD_MATERIAL:
			return SNAKE_HEAD;
			break;			
	}
	return FREE;		//to get rid of compiler warnings
}*/

CONTENTS map_ok(int **map, int curr_x, int curr_y, int max_x, int max_y, NSEW direction){
	switch(direction){
		case NORTH:	if(curr_y-1 < 0){
						return WALL;
					}else{
						return map[curr_y-1][curr_x];
					}
					break;
											
		case SOUTH:	if(curr_y+1 >= max_y){
						return WALL;
					}else{
						return map[curr_y+1][curr_x];
					}
					break;

		case EAST:	if(curr_x+1 >= max_x){
						return WALL;
					}else{
						return map[curr_y][curr_x+1];
					}
					break;

		case WEST:	if(curr_x-1 <0){
						return WALL;
					}else{
						return map[curr_y][curr_x-1];
					}
					break;
			
	}
	return FREE;		// just for get rid of warnings
}

int is_antagonic(int dir1, int dir2){
	switch(dir1){
		case NORTH:	if (dir2 == SOUTH) return 1;
						break;
		case SOUTH:	if (dir2 == NORTH) return 1;
						break;
		case EAST:	if (dir2 == WEST)	return 1;
						break;
		case WEST:	if (dir2 == EAST)	return 1;
						break;
	}

	switch(dir2){
		case NORTH:	if (dir1 == SOUTH) return 1;
						break;
		case SOUTH:	if (dir1 == NORTH) return 1;
						break;
		case EAST:	if (dir1 == WEST)	return 1;
						break;
		case WEST:	if (dir1 == EAST)	return 1;
						break;
	}
	return 0;
}

void ai_set_dirr(COORDS *to, SNAKE *snake){
	

	int x_diff = to->x - snake->p_head->position.x;
	int y_diff = to->y - snake->p_head->position.y;
	if (x_diff > 0){
		snake->next_direction = EAST;
	}else if(x_diff == 0){
		if (y_diff > 0){
			snake->next_direction = SOUTH;
		}else if(y_diff == 0){
			snake->next_direction = snake->curr_direction;
		}else{
			snake->next_direction = NORTH;
		}
	}else if(x_diff < 0){
		snake->next_direction = WEST;
	}
}
