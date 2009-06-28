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


#ifndef AI_H
#define AI_H

#include "snake.h"



// helper for get_closest_fruit()
int	measure_lenght(COORDS *from, COORDS *to);

// returns position of closest fruit
COORDS get_closest_fruit(SEGMENT *fruits, COORDS *curr_pos);

// sets direction of snake to get him to closest fruit - wery noninteligent :D
void ai_set_dirr(COORDS *to, SNAKE *snake);


#endif
