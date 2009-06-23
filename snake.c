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

/***snake.c***/

#define _BSD_SOURCE
#define _XOPEN_SOURCE 500

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "snake.h"
#include "common.h"


char DIRECTIONS[][10]={"north", "south", "east", "west"};
extern int g_speed;

void snake_init(SNAKE *p_snake, COORDS xy, NSEW direction, int map_body, int map_head){


	dputs("inside snake_init()");
	//printf("   :::given coords : [%2d,%2d]\n", xy.x, xy.y); 

	// setup snake properties
	p_snake->map_body = map_body;
	p_snake->map_head = map_head;
	//p_snake->num_of_lives = INIT_LIFES;	do this in main, we need this function to not to do this
	//p_snake->score = 0;					do this in main, we need to init snake in game flow(position)
	p_snake->color = INIT_COLOR;

	// allocate memory for segments
	int i;
	p_snake->p_head = (SEGMENT *)malloc(sizeof(SEGMENT));
	SEGMENT *p_seg_tmp = p_snake->p_head;

	for (i=1; i<INIT_LENGHT; i++){
		p_seg_tmp->p_next = (SEGMENT *)malloc(sizeof(SEGMENT));
		p_seg_tmp = p_seg_tmp->p_next;
		p_seg_tmp->p_next = NULL;
	}

	// place snake to apropriate position

	// head first
	p_snake->p_head->position = xy;

	// remaining segments		
	SEGMENT *p_tmp;
	int count = 0;

	for(p_tmp = p_snake->p_head; p_tmp != NULL; p_tmp = p_tmp->p_next){
#ifdef DEBUG_SNAKE
		printf("   :::inside cycle to get segments right coords, count = %d\n", count);
#endif
		switch (direction){
			case NORTH:	p_tmp->position.y = xy.y + count;	// place snake in given direction
						p_tmp->position.x = xy.x;
						break;
			case SOUTH:	p_tmp->position.y = xy.y - count;
						p_tmp->position.x = xy.x;
						break;
			case EAST:	p_tmp->position.x = xy.x - count;
						p_tmp->position.y = xy.y;
						break;
			case WEST:	p_tmp->position.x = xy.x + count;
						p_tmp->position.y = xy.y;
						break;
		}
		count++;
	}

	p_snake->curr_direction = direction;
	p_snake->next_direction = direction;

}// snake_init()

void snake_delete_mem(SNAKE *p_snake){

#ifdef DEBUG_SNAKE
	printf(":::snake_delete_mem()\n");
#endif

	// recursively do throught linked list of snakes body
	// and dealloc mem used for store segments coords
	SEGMENT *p_tmp_seg;
	SEGMENT *p_tmp_next;

	for ( p_tmp_seg = p_snake->p_head; p_tmp_seg->p_next != NULL; p_tmp_seg = p_tmp_next){
		p_tmp_next = p_tmp_seg->p_next;
		free( (void *)p_tmp_seg);
	}
	free( (void *)p_tmp_seg);
	p_snake->p_head = NULL;
}

void snake_add_segment(SNAKE *p_snake, int how_many){

#ifdef DEBUG_SNAKE
	printf(":::snake_add_segment()\n");
#endif

	// get current tail of our snake
	// than for tail->p_next alloc new memory
	SEGMENT	*p_tmp = p_snake->p_head;
	int i;

	for (p_tmp = p_snake->p_head; p_tmp->p_next != NULL;p_tmp = p_tmp->p_next)
		;
	// in p_tmp we have tail, tail->p_next == NULL

	// same code as in snake_init()
	for (i=0; i<how_many; i++){
		p_tmp->p_next = (SEGMENT *)malloc(sizeof(SEGMENT));
		p_tmp->p_next->position = p_tmp->position;
		p_tmp = p_tmp->p_next;
		p_tmp->p_next = NULL;
	}
}

// help function for snake_move()
void coords_switch(COORDS *p_c1, COORDS *p_c2){

#ifdef DEBUG_SNAKE
	printf(":::coords_switch()\n");
	printf("  --switching [%d,%d] and [%d,%d]\n",
			p_c1->x, p_c1->y,
			p_c2->x, p_c2->y);
#endif

	COORDS tmp;
	tmp	= *p_c1;
	*p_c1 = *p_c2;
	*p_c2 = tmp;
}

void snake_move(SNAKE *p_snake){
	// there we go through snakes body and travel values(x,y) from head 
	// to tail. Tail coordinates are thrown away - we dont need them anymore
	// From direction and from the coordinates of snakes head we get
	// coordinated for new position of head.

#ifdef DEBUG_SNAKE
	printf(":::snake_move()\n");
#endif




	// calculate new coords of head
	COORDS tmp_xy = p_snake->p_head->position;

	int move_ok = 1;

	switch (p_snake->next_direction){
		case NORTH:	if( !(p_snake->curr_direction == SOUTH)){
						tmp_xy.y--;
						//puts("   :::switch (direction)\n      head to north");
					}else{
						// we cannot move
						move_ok = 0;
					}
					break;
		case SOUTH:	if( !(p_snake->curr_direction == NORTH)){
						tmp_xy.y++;
						//puts("   :::switch (direction)\n      head to south");
					}else{
						move_ok = 0;
					}
					break;
		case EAST:	if( !(p_snake->curr_direction == WEST)){
						tmp_xy.x++;
						//puts("   :::switch (direction)\n      head to east");
					}else{
						move_ok = 0;
					}
					break;
		case WEST:	if( !(p_snake->curr_direction == EAST)){
						tmp_xy.x--;
						//puts("   :::switch (direction)\n      head to west");
					}else{
						move_ok = 0;
					}
					break;
	}
	// in tmp_xy we now have coords, where should be head of snake

#ifdef DEBUG_SNAKE
	printf("      head is now at coords  [%d,%d]\n", p_snake->p_head->position.x, p_snake->p_head->position.y);
	printf("      head will be placed in [%d,%d]\n", tmp_xy.x, tmp_xy.y);
#endif
		
	if (move_ok){

			// move body
			SEGMENT *curr_segment = p_snake->p_head;

			do{
				//printf(">>move_ok : curr_seg - [%d,%d]\n", curr_segment->position.x, curr_segment->position.y);
				coords_switch(&curr_segment->position, &tmp_xy);
				curr_segment = curr_segment->p_next;
			}while( curr_segment != NULL);

			// set new direction
			p_snake->curr_direction = p_snake->next_direction;
		}else{
			puts("EE:not moving - got antagonistic direction");
		}
	
}

// returns lenght of body
int snake_get_lenght(SNAKE *p_snake){

#ifdef DEBUG_SNAKE
	printf(":::snake_get_lenght\n");
#endif

	int len=0;
	SEGMENT *p_tmp;
	for(p_tmp = p_snake->p_head; p_tmp != NULL; p_tmp = p_tmp->p_next){
		len++;
	}

	return len;
}

void snake_describe(SNAKE *p_snake){

#ifdef DEBUG_SNAKE
	printf(":::snake_describe()\n");
#endif	

	// properties
	printf("snake decription :\ndirection: %s\nlifes: %d\nscore: %d\ncolor: %d\n",
			DIRECTIONS[p_snake->curr_direction],
			p_snake->num_of_lifes, 
			p_snake->score,
			p_snake->color
			);

	// coordinates
	int count = 1;
	SEGMENT *p_tmp;

	for(p_tmp = p_snake->p_head; p_tmp != NULL; p_tmp = p_tmp->p_next){
		printf("%d. segment - [%d,%d]\n", count, p_tmp->position.x, p_tmp->position.y);
		count++;
	}
}

int snake_score(SNAKE *p_snake, int score){
	p_snake->score += score;
	return p_snake->score;
}
