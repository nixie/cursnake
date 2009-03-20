/** snake.h **/
#ifndef SNAKE_H
#define SNAKE_H



/************ some constants we need ***/
#define INIT_LENGHT		4		// minimal value is 1
#define INIT_LIFES		3
#define INIT_COLOR	1
#define BODY_MATERIAL	'x'
#define HEAD_MATERIAL	'x'

#define SNAKE_NAME_LEN	20

/************ structure defs ***********/

// snake body is a linked list ended with NULL pinter to next item
typedef struct coords{
	int x;
	int y;
} COORDS;

typedef struct segment{
	COORDS position;					
	struct segment *p_next;	// next segment of snakes body
} SEGMENT;

typedef enum{
	NORTH=0, 
	SOUTH, 
	EAST, 
	WEST
} NSEW;


typedef struct snake{
	SEGMENT *p_head;
	NSEW curr_direction;
	NSEW next_direction;
	int num_of_lifes;
	int score;
	int color;
	int map_body;
	int map_head;
	char name[SNAKE_NAME_LEN];
} SNAKE;


/*********** f. declarations *************/
// setup properties and alloc mem
void snake_init(SNAKE *p_snake, COORDS xy, NSEW direction, int map_body, int map_head);	

// dealloc whole snake
// after this we should call: snake = NULL;
void snake_delete_mem(SNAKE *p_snake);	

// prolong tail
void snake_add_segment(SNAKE *p_snake, int how_many);

// move whole body after head, we have to test if we could
// move there before
void snake_move(SNAKE *p_snake);

// returns lenght of body
int snake_get_lenght(SNAKE *p_snake);

// for debugg purpose, print all info about given object
void snake_describe(SNAKE *p_snake);

// returns current score
int snake_score(SNAKE *snake, int increment);

#endif
