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

#include "snake.h"
#include "main.h"
#include "arena.h"
#include "ai.h"



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
