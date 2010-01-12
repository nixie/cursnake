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

/*** MAIN.c for snake game ***/
#define _BSD_SOURCE
#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <curses.h>
#include <string.h>

#include "debug.h"
#include "arena.h"
#include "ai.h"
#include "main.h"

char *main_menu[] = {
	//first item in menu is name of the menu
	//first charactes of menu entry name are used for switches
	//	! in *menu[0] meens that this is switch (setting bool values)
	//	0,1 after ! informs in which state the switch is
	//	menu_show then returns in which state was the menu entry before <enter>
	//	if switch is on, then menu_show() will highlight it in some special way
	"> Welcome to cursnake",
	"play",
	"game type",
	"difficulty",
	"player setup",
	"credits",
	"exit",
	NULL,
};

char *difficulty_menu[] = { 
	"> choose difficulty",
	"beginner",
	"advanced",
	"profi",
	NULL,
};

char *player_setup_menu[] = {
	"> choose number of players",
	"one",
	"two",
	NULL,
};

char *game_type_menu[] = {	// order and order matters - changing 
	// switch value uses absolute access
	"> choose gamestyle",
	"classical  snake", 		// CLASSIC
	"!friendly walls",			// NOWALLS
	"fruit collecting", 		// FISHES
	"tron-like style",			// TRON
	NULL,
};

char status_text[STATUS_TEXT_LEN];

// this variable are used by set_cli_opts() which set it to approp values.
int g_speed;	
int g_number_of_players;
int g_fruits;
int g_gamestyle;
int g_mod_no_walls;
int g_show_info;
int g_autopilot;
int g_game_pause;

// players keys
int keys_pl1[4];
int keys_pl2[4];


/*************************************************************************/

void init_defaults(void){
	g_speed = PROFI_SPEED;
	g_number_of_players = 1;
	g_fruits = INIT_FRUITS;
	g_gamestyle = CLASSIC;
	g_mod_no_walls = 0;
	g_show_info=0;
	g_autopilot = 0;
	g_game_pause = 0;

	keys_pl1[0] = KEY_UP;
	keys_pl1[1] = KEY_DOWN;
	keys_pl1[2] = KEY_RIGHT;
	keys_pl1[3] = KEY_LEFT;

	keys_pl2[0] = 'w';
	keys_pl2[1] = 's';
	keys_pl2[2] = 'd';
	keys_pl2[3] = 'a';
}


int main(int argc, char *argv[])
{
	init_defaults();

	if (set_cli_opts(argc, argv) != 0){
		return 0;
	}

#ifndef NDEBUG
   // debuging enabled - truncate file LOG_FILE
   FILE *fw = fopen(LOG_FILE, "w");
   if (fw == NULL){
      fprintf(stderr, "attempt to truncate size of logfile failed\n");
   }
#endif



	//init curses
	WINDOW *p_root;

	if ( (p_root = initscr()) == NULL){
		fprintf(stderr, "screen cannot be initialized, ending\n");
	}
	if (!has_colors()) {
		endwin();
		fprintf(stderr, "Error - no color support on this terminal\n");
		exit(1);
	}

	if (start_color() != OK) {
		endwin();
		fprintf(stderr, "Error - could not initialize colors\n");
		exit(2);
	}
	init_pair(1, COLOR_BLUE, COLOR_BLACK);		// player1
	init_pair(2, COLOR_RED, COLOR_BLACK);		// player2, highlighted menu entry
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);	// non_higlighted menu entry, credits
	init_pair(4, COLOR_BLACK, COLOR_WHITE );	// status line
	init_pair(5, COLOR_GREEN, COLOR_BLACK);		// toggle menu switch highlited


	curs_set(0);		// no cursor

	int game = 1;
	int options;




	while(game){// main - menu loop
      pmesg(D_MENU, "entering base menu switch\n");
		switch( show_menu(main_menu) ){
         case PLAY:	pmesg(D_MENU, "  case:PLAY\n");
						start_game();// game loop
						break;

			case GAMESTYLE:	pmesg(D_MENU, "  case:GAMESTYLE\n");
							switch(show_menu(game_type_menu)){
								case CLASSIC:	g_gamestyle = CLASSIC;
												break;
								case NOWALLS:	if (g_mod_no_walls){
													g_mod_no_walls = 0;
												}else{
													g_mod_no_walls = 1;
												}	
												break;
								case FISHES:	g_gamestyle = FISHES;
												break;
								case TRON:		g_gamestyle = TRON;
												break;
								case -1:		break;
							}
							break;

			case PLAYER_SETUP:	pmesg(D_MENU, "  case:PLAYER_SETUP\n");
								switch(show_menu(player_setup_menu)){
									case ONE:	g_number_of_players = 1;
												break;
									case TWO:	g_number_of_players = 2;
												break;
									case -1:	break;
								}
								break;

			case DIFFICULTY: pmesg(D_MENU, "  case:DIFFICULTY\n");
							 switch(show_menu(difficulty_menu)){
								 case BEGINNER:	g_speed = BEGINNER_SPEED;
												g_fruits = 10;
												break;
								 case ADVANCED:	g_speed = ADVANCED_SPEED;
												g_fruits = 30;
												break;
								 case PROFI:		g_speed = PROFI_SPEED;
													g_fruits = 50;
													break;
								 case -1:		options = 0;
												break;
							 }
							 break;
			case CREDITS:	pmesg(D_MENU, "  case:CREDITS\n");
							rprint_credits(stdscr);
							//flush_input();
							getch();
							break;						
			case -1:	

			case EXIT:	pmesg(D_MENU, "  case:EXIT or -1\n");
						game=0;
						break;

			default:	pmesg(D_MENU | D_ERROR, "function show_menu() returned nonenumerated value!\n");
						break;
		}
	}

	// clean up curses
   pmesg(D_BASE, "cleaning up curses\n");
	endwin();

   pmesg(D_BASE, "exiting game\n");
	return EXIT_SUCCESS;

}//main

int set_cli_opts(int argc, char *argv[]){

	int opt;

	while ( (opt=getopt(argc, argv, "s:f:hiw")) != -1){
		switch(opt){
			case 's':	g_speed = (1 / (float)atoi(optarg) ) * 1000;
						break;

			case 'f':	g_fruits = atoi(optarg);
						break;

			case 'h':	printf("\nCursnake - the curses snake game\n   usage: %s [OPTIONS]\n\n", GAME_NAME);
						printf("\t-s SPEED\tSet initial speed\n\t-f NUM\t\tSet initial number of fruits\n");
						printf("\t-w \t\tno walls, no crashes\n");
						printf("\t-i \t\twhen playing, show info about everything\n\t-h \t\tShow this help\n");
						return 1;
						break;
			case 'i':	g_show_info = 1;
						break;

			case 'w':	g_mod_no_walls = 1;
						break;

			case ':':	printf("option needs a value\n");
						return 1;
						break;

			case '?':	printf("unknown option: %c\n", optopt);
						printf("for list of known options use %s -h\n", GAME_NAME);
						return 1;
						break;
		}
	}
	return 0;
}

int get_text_count(char **menu, int *lines, int *line_max_len){
	// counts number of lines we need for menu
	// counts minimal size of cols we need
	int count=0;
	int max_len=0;
	int tmp_len=0;

	while(menu[0] != NULL){
		if ((tmp_len = strlen(menu[0])) > max_len ){
			max_len = tmp_len;	
		}

		menu++;
		count++;
	}

	*lines = count;
	*line_max_len = max_len;
	return count;	
}


int show_menu(char **contents)
{
	pmesg(D_MENU, "show_menu started\n");
	int menu_lines,
		menu_max_len;

	get_text_count(contents, &menu_lines, &menu_max_len);		

	WINDOW *p_menu_window = newwin( menu_lines + MENU_ROWS_ADD, menu_max_len + MENU_COLS_ADD, MENU_STARTY, MENU_STARTX);


	box(p_menu_window, 0, 0);

	if ( p_menu_window == NULL){
		pmesg(D_MENU | D_ERROR, "couldnt create menu window\n");
	}

	int key=0;
	int highlited=0;

	keypad(stdscr, TRUE);
	clear();
	noecho();
	cbreak();
	nodelay(stdscr, FALSE);

	while(1){	// draw and wait for key 
		// print menu
		int i;

		// something like heading - bold it
		if(	(int)wattrset(p_menu_window, COLOR_PAIR(2) | A_BOLD| A_UNDERLINE) == ERR){		// highlited
			pmesg(D_MENU | D_ERROR, "heading color set failed\n");
		}
		mvwprintw(p_menu_window, 1, 1, contents[0]);		// print headline
		wattrset(p_menu_window, COLOR_PAIR(1));				// normal

		char **p_p_menu = contents;			

		p_p_menu++;		// first is headline, so skip it
		for(i=0; *p_p_menu != NULL; i++){

			//if (wattrset(p_menu_window, COLOR_PAIR(1) | A_BOLD ) != OK){	// default values
			//	pmesg(D_MENU | D_ERROR, "init	colors failtd\n");
			//}

			// there we go through menu items
			if(i == highlited ){				
				if ((int)wattrset(p_menu_window, COLOR_PAIR(2) | A_BOLD ) == ERR){
					pmesg(D_MENU | D_ERROR, "attron failed\n");
				}

				//pmesg(D_MENU, "printing highlighted menu entry\n");
				// test for switch
				if (**p_p_menu == '!'){
					// show only entry not ![01]
					mvwprintw(p_menu_window, 3+i, 2, (*p_p_menu)+1);
				}else{
					// this is not a switch
					mvwprintw(p_menu_window, 3+i, 2, (*p_p_menu));
				}

			}else{ // non highlited

				if ((int)wattrset(p_menu_window, COLOR_PAIR(3) ) == ERR){
					pmesg(D_MENU | D_ERROR, "attron failed\n");
				}

				if (**p_p_menu == '!' ){ 
					// this is a switch
					// TODO: nouzovka
					if (g_mod_no_walls != 0){ 
						pmesg(D_MENU, "printing switch[on]\n");
						wattrset(p_menu_window, COLOR_PAIR(5) );
						mvwprintw(p_menu_window, 3+i, 2, (*p_p_menu)+1);
					}else{				
						// it is a switch and is off
						pmesg(D_MENU, "printing switch[off]\n");
						mvwprintw(p_menu_window, 3+i, 2, (*p_p_menu)+1);
					}
				}else{
					// it is normal entry 
					mvwprintw(p_menu_window, 3+i, 2, *p_p_menu);
				}

			}
			p_p_menu++;
		}

		//pmesg(D_MENU, "menu printed, wrefresh()\n");
		//touchwin(p_menu_window);
		refresh();
		touchwin(p_menu_window);
		wrefresh(p_menu_window);
		// wait for key
		key = getch();	

		//pmesg(D_MENU, "menu - some key was pressed\n");

		if (key == KEY_UP || key == 'k'){
			pmesg(D_MENU, "   up key pressed\n");
			if(highlited == 0){
				highlited = menu_lines -2;
			}else{		
				highlited--;
			}
		}else if (key == KEY_DOWN || key == 'j'){
			pmesg(D_MENU, "   down key pressed\n");
			if(highlited == menu_lines-2){
				highlited = 0;
			}else{
				highlited++;
			}
		}else if (key == KEY_ENTER || key == '\n'){
			delwin(p_menu_window);
			pmesg(D_MENU, "   enter key pressed\n");
			return highlited;
		}else if (key == 'q'){
			delwin(p_menu_window);
         pmesg(D_MENU, "   q key pressed\n");
			return -1;
		}else if (key == 27){		// escape key
			delwin(p_menu_window);
         pmesg(D_MENU, "   esc key pressed\n");
			return -1;
		}else if (key == ERR){
			pmesg(D_MENU, "   whats this?got it"); // TODO
		}

	}//menu loop
}//show_menu()

void flush_input(void){

	nodelay(stdscr, TRUE);

	while ( wgetch(stdscr) != ERR)
		;

}

int get_last_keys(KEYPAIR_T *p_keys){
	cbreak();
	int key=0;

	p_keys->key1 = ERR;
	p_keys->key2 = ERR;

	while ( (key = wgetch(stdscr)) != ERR){
		if ( key == keys_pl1[0] || key == keys_pl1[1] || key == keys_pl1[2] || key == keys_pl1[3]){
			p_keys->key1 = key;
		}else if( key == keys_pl2[0] || key == keys_pl2[1] || key == keys_pl2[2] || key == keys_pl2[3]){
			p_keys->key2 = key;
		}else if( key == 'q' ){
			// quit match
         pmesg(D_BASE, "q key pressed - quitting round\n");
			return 'q';
		}else if( key == KEY_F(1)){
         pmesg(D_BASE | D_ARENA, "F1 key pressed - toggling showing info\n");
			if (g_show_info){
				g_show_info =0;
			}else{
				g_show_info =1;
			}
		}else if( key == KEY_F(2)){
         pmesg(D_BASE | D_SNAKE, "F1 key pressed - toggling autopilot\n");
			if (g_autopilot){
				g_autopilot = 0;
			}else{
				g_autopilot = 1;
			}

		}else if( key == 'p'){
			//togle pause
         pmesg(D_BASE, "p key pressed - toggling pause\n");
			if (g_game_pause == 1){
				g_game_pause = 0;
			}
			else{
				g_game_pause = 1;
			}
		}
	}

	return ERR;
}


int start_game(void){
	clear();
	cbreak();
	noecho();


	unsigned int cols,rows;

	// create arena window
	WINDOW *p_arena = newwin (0, 0, 0, 0);	// whole screen
	if (!p_arena){
		pmesg(D_BASE | D_ERROR | D_ARENA, "cannot create arena screen");
		return 1;
	}
	box(p_arena, 0, 0);

	getmaxyx(p_arena, rows, cols); 		// in rows, cols are dimensions now
	char text_buffer[BUFF_MAX];					// to sprintf data and then status is

	keypad(p_arena, TRUE);

	int **p_p_map = alloc_map(rows-2, cols-2 );	
	if (p_p_map == NULL){
      pmesg(D_BASE | D_ARENA | D_ERROR, "Not enough memory for map\n");
		mvprintw(0,0,"Not enough memory for map, press any key...");
		delwin(p_arena);
		refresh();
      getch();    // wait for keypress, so user can read error message
		return 1;
	}

	clear_map(p_p_map, cols-2, rows-2);

	pmesg(D_BASE | D_SNAKE, "creating snake objects\n");

	SNAKE player1;	
	SNAKE player2;
	player1.num_of_lifes = INIT_LIFES;
	player2.num_of_lifes = INIT_LIFES;
	strcpy(player1.name, "blue");
	strcpy(player2.name, " red");
	player1.score = 0;
	player2.score = 0;
	player1.p_head = NULL;
	player2.p_head = NULL;

	COORDS player1_coords={ 10,(rows-2)/2};		// c,r    (x,y)	
	COORDS player2_coords= { cols-10, (rows-2)/2};


	COORDS arena_coords_max={cols-2, rows-2};
	SEGMENT *p_fruits = NULL;

	int game_state=2;	// 1 -normal cycle, 2 -start from begining, 0 -end
	int no_wait_q_pressed = 0;

	int fruits_remaining=1;

	KEYPAIR_T keys;

	pmesg(D_BASE, "game cycle started\n");
	while(game_state >= 1){		 
		//pmesg("move cycle started");
		clear_map(p_p_map, cols-2, rows-2);	
		status_text[0] = '\0';
		// set keyboard to be nonblocking
		// create map to represent content of every [x,y]
		// create snake object
		// create linked list of fruits
		// then -count fruits, if fruits.count == 0 ->print_score(score);
		// 		-draw fruits (map)
		// 		-draw snake
		// 		-find key pressed, if nothing pressed, continue in curr_direction
		// 		-find what is the next [x,y]. If WALL -> return GAME_END
		// 									  If FRUIT-> score snake, remove fruit from map also
		//		-mvwprintw(p_arena, 10, 0, "score : %d", snake.score);
		// 		-wait a while
		if ( game_state == 2){
			// reinit

			if (player1.p_head != NULL){
				snake_delete_mem(&player1);
			}
			snake_init(&player1, player1_coords, EAST, PL1_BODY, PL1_HEAD);

			if (g_number_of_players == 2){
				if(player2.p_head != NULL){
					snake_delete_mem(&player2);
				}
				snake_init(&player2, player2_coords, WEST, PL2_BODY, PL2_HEAD);
			}

			// reinit fruits
			if (p_fruits != NULL){
				free_fruits( p_fruits);
			}

			p_fruits = init_fruits( g_fruits, arena_coords_max, p_p_map);
			game_state = 1;
		}





		snake_to_map( &player1, p_p_map, cols-2, rows-2);

		if (g_number_of_players == 2){
			snake_to_map( &player2, p_p_map, cols-2, rows-2);
		}

		//if (g_gamestyle == CLASSIC || g_gamestyle == FISHES ){
		fruits_to_map(p_fruits, p_p_map, cols-2, rows-2);	
		//}

		render_map(p_p_map, p_arena, cols-2, rows-2 );


		if (g_show_info){

			wattrset(p_arena, A_NORMAL);
			// find out closest fruit and print its coords
			COORDS tmpc = get_closest_fruit(p_fruits, &(player1.p_head->position));
			mvwprintw(p_arena, 1,1, "closest[%d,%d]", 
					tmpc.x, 
					tmpc.y);
			int l = measure_lenght(&player1.p_head->position, &tmpc);
			int i;
			for (i=1; i<=l; i++){
				mvwaddch(p_arena, rows-2, i, (chtype)'-');
			}
			// print head coords
			mvwprintw(p_arena, 3,1, "   head[%d,%d]", 
					player1.p_head->position.x,
					player1.p_head->position.y);
			//mvwprintw(p_arena, 3,1, "abs(headx-fruitx):%d", 
			//		abs(player1.p_head->position.x - tmpc.x));
			//mvwprintw(p_arena, 4,1, "abs(heady-fruity):%d",
			//		abs(player1.p_head->position.y - tmpc.y));

			//COORDS h = {25,3};
			//COORDS l = {36,35};
			//COORDS j = {27,33};
			//mvwprintw(p_arena, 5,1, "mltest:[25,3]--[36,35]: %d",
			//	measure_lenght(&h, &l));
			//mvwprintw(p_arena, 6,1, "mltest:[27,33]--[36,35]: %d",
			//		measure_lenght(&j, &l));
			//mvwprintw(p_arena, 7,1, "mltest:[36,35]--[27,33]: %d",
			//		measure_lenght(&l, &j));

			print_fruit_coords(p_fruits, p_arena, player1.p_head->position);
		}



		if (g_gamestyle == CLASSIC || g_gamestyle == FISHES){
			fruits_remaining = fruits_count( p_fruits);

			if ( fruits_remaining == 0){
				game_state = 0;
			}
		}



		nodelay(stdscr, TRUE);

		if ( get_last_keys(&keys) == 'q'){
			// quit match
			player1.score = 0;
			player2.score = 0;
			player1.num_of_lifes = 0;
			player2.num_of_lifes = 0;
			game_state = 0;
			no_wait_q_pressed = 1;
		}

		// if game is paused, do nothing
		if (g_game_pause){
			usleep(100000);
			continue;
		}

		if (keys.key1 == ERR){
			// pl1 now pressed nothing
			player1.next_direction = player1.curr_direction;
		}else{
			if ( is_antagonic( key1_to_NSEW(keys.key1), player1.curr_direction)){
				player1.next_direction = player1.curr_direction;
			}else{
				player1.next_direction = key1_to_NSEW(keys.key1);
			}
		}

		// if we play in mutiplayer
		if (g_number_of_players == 2){
			if (keys.key2 == ERR){
				// pl1 now pressed nothing
				player2.next_direction = player2.curr_direction;
			}else{
				if ( is_antagonic( key2_to_NSEW(keys.key2), player2.curr_direction)){
					player2.next_direction = player2.curr_direction;
				}else{
					player2.next_direction = key2_to_NSEW(keys.key2);
				}
			}
		}

		if (g_autopilot){
			COORDS go_to;
			if (g_number_of_players ==2){
				// player2 is computer
				go_to = get_closest_fruit(p_fruits, &player2.p_head->position);
				ai_set_dirr(&go_to, &player2);
				if (is_antagonic(player2.curr_direction, player2.next_direction)){
					player2.next_direction = player2.curr_direction;
				}
			}


			go_to = get_closest_fruit(p_fruits, &(player1.p_head->position));
			ai_set_dirr(&go_to, &player1);
			if (is_antagonic(player1.curr_direction, player1.next_direction)){
				// this 
				player1.next_direction = player1.curr_direction;
				// maked snake go straight on, endlesly
				//
				// We have turn left or right. Whether turn left or right depends on
				// nothing, for now :)
				/*if (player1.next_direction == NORTH || player1.next_direction == SOUTH){
					player1.next_direction = EAST;
				}else{
					player1.next_direction = SOUTH;
				}*/
			}

		}



		CONTENTS next_content;
		switch (next_content = map_ok(p_p_map, player1.p_head->position.x, player1.p_head->position.y, cols-2, rows-2, player1.next_direction)){
			case FREE:			if (g_gamestyle == TRON){
									snake_add_segment(&player1, 1);
								}
								snake_move(&player1);
								break;
			case PL1_BODY:
			case PL1_HEAD:
			case PL2_BODY:		
			case PL2_HEAD:
			case WALL:			if (g_mod_no_walls == 0){
									player1.num_of_lifes--;
									usleep(LIFE_DOWN_DELAY_US);
									game_state = 2;
								}
								if (g_autopilot){
									// what to do when thare is a collision situation?
									// ai should do this
									//  1. not hit the walls, turn somewhere else.
									//  2. not hit the oponents body, turn somewhere else.
									//  3. if we are going to hit our own body in next step
									//     do this: get nuber of body segment (seg_num) we are against.
									//     Head has number 1. Next, go in the right direction
									//     to not to finish in self loop - try to find the tail...
									//     so increment the number of body segment we are againt and
									//     go to some free place near this [seg_num+1] segment.
									//
									// so find other possible direction ai_set_dir2()
									// and if the case is OUR_BODY dont run in self loop(intead of
									// ai_set_dir2() use ai_set_dir_noloop().
									// 
									// to protect from self loops we need function to :
									// 	- if ai_set_dirr() get us in trouble (she is primitive), call better,
									// 	  but limited ai_set_dir_noloop() which only sees litle around head
									//  - function which returns seg_num of segment on concrete position
									//  - function which returns position of segment with concrete seg_num
									//  - function which gets new direction from seg_num.position - seg_num+1.position
									//  last 2 are helpers for ai_set_dir_noloop()
									//  all is defined in ai.c
									
									int move_ok =0;


									// test for selfloop
									if (next_content == PL1_BODY)
									{
										COORDS seg_pos = player1.p_head->position;
										COORDS seg_next_pos = {-1,-1};

										// find what seg_num we are against.
										// this could be a function
										switch (player1.next_direction){
											case NORTH: seg_pos.y--;
														break;
											case SOUTH: seg_pos.y++;
														break;
											case EAST:	seg_pos.x++;
														break;
											case WEST:	seg_pos.x--;
														break;
										}


										// go throught segments, until we find some with this position,
										// then save position of next segment.
										
										SEGMENT *p_temp;
										for(p_temp = player1.p_head; p_temp->p_next != NULL; p_temp = p_temp->p_next){
											if ( (p_temp->position.x == seg_pos.x) && (p_temp->position.y == seg_pos.y)){
												// this is the segment we are standing against
												seg_next_pos = p_temp->p_next->position;
												break;
											}else{
											}
												
										}
										
										
										// check if there was such segment (if wasnt, there is bad mistake in some code)
										if (seg_next_pos.x == -1){
											pmesg(D_ARENA | D_SNAKE | D_ERROR, "fatal error, exiting\n");
											//game_state = 0;
										}
										
										// now we have a vector (seg_pos - seg_next_pos), we find direction
										// where to go, to get closest to tail.
										NSEW dir;
										int curve=0; // counter of directions setted (if we write dir twice, 
													 // we have to do further processing...

										
										if (seg_pos.x > seg_next_pos.x){
											dir = WEST;
											curve++;
										}else if(seg_pos.x < seg_next_pos.x){
											dir = EAST;
											curve++;
										}else if(seg_pos.x == seg_next_pos.x){
											// vertical barrier
											if (seg_pos.y > seg_next_pos.y){
												dir = NORTH;
												curve++;
											}else if(seg_pos.y < seg_next_pos.y){
												dir = SOUTH;
												curve++;
											}else{
												// seg_pos == seg_next_pos, which is nonreal
												pmesg(D_ARENA | D_SNAKE | D_ERROR, "fatal error seg_pos == seg_next_pos, exiting\n");
												//game_state = 0;
											}
										}

										// try to move
										if (map_ok(p_p_map, player1.p_head->position.x, player1.p_head->position.y, cols-2, rows-2,dir) == FREE){
											player1.next_direction=dir;
											move_ok=1;
										}										
										
									}
									else
									{
										if (player1.curr_direction == EAST || player1.curr_direction == WEST){
											if (map_ok(p_p_map, player1.p_head->position.x, player1.p_head->position.y, cols-2, rows-2, SOUTH) == FREE){
												player1.next_direction = SOUTH;
												move_ok=1;
											}else if(map_ok(p_p_map, player1.p_head->position.x, player1.p_head->position.y, cols-2, rows-2, NORTH) == FREE){
												player1.next_direction = NORTH;
												move_ok=1;
											}
										}else if(player1.curr_direction == SOUTH || player1.curr_direction == NORTH){
											if (map_ok(p_p_map, player1.p_head->position.x, player1.p_head->position.y, cols-2, rows-2, EAST) == FREE){
												player1.next_direction = EAST;
												move_ok=1;
											}else if(map_ok(p_p_map, player1.p_head->position.x, player1.p_head->position.y, cols-2, rows-2, WEST) == FREE){
												player1.next_direction = WEST;
												move_ok=1;
											}
										}
									}


									if (move_ok){
										snake_move(&player1);
									}else if(map_ok(p_p_map, player1.p_head->position.x, player1.p_head->position.y, cols-2, rows-2, player1.curr_direction) == FREE){
										player1.next_direction = player1.curr_direction;
										snake_move(&player1);
									}else{
										// try move wherever if FREE
										
										if (player1.curr_direction == EAST || player1.curr_direction == WEST){
											if (map_ok(p_p_map, player1.p_head->position.x, player1.p_head->position.y, cols-2, rows-2, SOUTH) == FREE){
												player1.next_direction = SOUTH;
												move_ok=1;
											}else if(map_ok(p_p_map, player1.p_head->position.x, player1.p_head->position.y, cols-2, rows-2, NORTH) == FREE){
												player1.next_direction = NORTH;
												move_ok=1;
											}
										}else if(player1.curr_direction == SOUTH || player1.curr_direction == NORTH){
											if (map_ok(p_p_map, player1.p_head->position.x, player1.p_head->position.y, cols-2, rows-2, EAST) == FREE){
												player1.next_direction = EAST;
												move_ok=1;
											}else if(map_ok(p_p_map, player1.p_head->position.x, player1.p_head->position.y, cols-2, rows-2, WEST) == FREE){
												player1.next_direction = WEST;
												move_ok=1;
											}
										}
										if (move_ok){
											snake_move(&player1);
										}
									}


									

								}


								break;
			case FRUIT:			snake_score(&player1, SCORE_INCREMENT);
								snake_move(&player1);
								remove_fruit(p_fruits, player1.p_head->position);
								if (g_gamestyle != FISHES){
									snake_add_segment(&player1, BODY_INCREMENT);
								}
								break;
		}

		if (g_number_of_players == 2){
			// do the same for second player
			switch (map_ok(p_p_map, player2.p_head->position.x, player2.p_head->position.y, cols-2, rows-2, player2.next_direction)){
				case FREE:			if (g_gamestyle == TRON){
										snake_add_segment(&player2, 2);
									}		
									snake_move(&player2);
									break;
				case PL1_BODY:	
				case PL2_HEAD:
				case PL2_BODY:		
				case PL1_HEAD:
				case WALL:			if (g_mod_no_walls == 0){
										player2.num_of_lifes--;
										usleep(LIFE_DOWN_DELAY_US);
										game_state = 2;
									}
									// todo:better ai for 2nd player also
									break;
				case FRUIT:			snake_score(&player2, SCORE_INCREMENT);
									snake_move(&player2);
									remove_fruit(p_fruits, player2.p_head->position);
									if (g_gamestyle != FISHES){
										snake_add_segment(&player2, BODY_INCREMENT);
									}
									break;
			}
		}

		// set colors to status line colors and leave message
		wattrset(p_arena, COLOR_PAIR(4) | A_STANDOUT | A_BOLD);

		// test for zero lifes and update status info
		if (g_number_of_players == 1){
			if (player1.num_of_lifes == 0){
				game_state = 0;
			}else{
				snprintf(text_buffer, BUFF_MAX, "%s(score:%d,lifes:%d,curr_d:%d,next_d:%d)", player1.name, player1.score, player1.num_of_lifes, player1.curr_direction, player1.next_direction);
				strncpy(status_text, text_buffer, STATUS_TEXT_LEN);
				//status_add_s(buffer);
			}
		}else if(g_number_of_players == 2){
			if (player2.num_of_lifes == 0 || player1.num_of_lifes == 0){
				game_state = 0;
			}else{
				snprintf(text_buffer, BUFF_MAX, "%s(score:%d,lifes:%d)   ", player1.name, player1.score, player1.num_of_lifes);
				strncpy(status_text, text_buffer, STATUS_TEXT_LEN);
				snprintf(text_buffer, BUFF_MAX, "%s(score:%d,lifes:%d)", player2.name, player2.score, player2.num_of_lifes);
				strncat(status_text, text_buffer, STATUS_TEXT_LEN);
			}
		}

		if (strlen(status_text) > cols){
			mvwprintw(p_arena, 0, 2, "too little space for statusline");
		}else{
			mvwprintw(p_arena, 0, 2, status_text);
		}



		refresh();	
		wrefresh(p_arena);

		usleep(g_speed*1000);

	}

	wclear(p_arena);

	if (g_number_of_players == 1){
		player1.num_of_lifes > 0 ? 	mvwprintw(p_arena, (rows-2)/2, 10, "You've made it !") :
			mvwprintw(p_arena, (rows-2)/2, 10, "You did NOT made it !");
	}else if(g_number_of_players ==2){
		// test who is the winner
		if (player1.num_of_lifes == player2.num_of_lifes){
			if (player1.score == player2.score){
				mvwprintw(p_arena, (rows-2)/2, 10, "nobody wins !");
			}else{
				player1.score > player2.score ?	mvwprintw(p_arena, (rows-2)/2, 10, "%s wins!", player1.name):
					mvwprintw(p_arena, (rows-2)/2, 10, "%s wins!", player2.name);
			}
		}else{
			player1.num_of_lifes > player2.num_of_lifes ?	mvwprintw(p_arena, (rows-2)/2, 10, "%s wins!", player1.name):
				mvwprintw(p_arena, (rows-2)/2, 10, "%s wins!", player2.name);
		}
	}

	wrefresh(p_arena);
	no_wait_q_pressed == 1 ?
		wait_for_key(0):
		wait_for_key(1);


	free_fruits( p_fruits);
	dealloc_map(p_p_map, rows -2);
	delwin(p_arena);
	return 0;
}

int get_longest_line_len(FILE *fr){
	int buff_size = 100;
	char buff[buff_size];
	int max=0, tmp;

	while ( fgets(buff, buff_size, fr ) != NULL ){
		if ( (tmp =strlen(buff)) > max){
			max = tmp;
		}
	}
	rewind(fr);			// set stream to start of file
	return max;
}

void rprint_credits(WINDOW *win){
	FILE *fr;
	if ( (fr = fopen(CREDITS_FILE, "r")) == NULL){
		mvwprintw(win, 3,3, "error : cannot open file %s", CREDITS_FILE);
		mvwprintw(win, 4,3,	"this file should be in directory with this executable");
		refresh();
		getch();
	}else{

		clear();
		int row_count = 1;				// on which row we start printing

		int buff_size = 100;
		int win_len = getmaxx(win);				
		int len;

		int col_offset;					// place output in the middle of window

		char buff[buff_size];
		wattrset(win, COLOR_PAIR(3) | A_BOLD);
		while ( fgets(buff, buff_size, fr ) != NULL ){	// print it

			len = strlen(buff);
			col_offset = (win_len - len) / 2;			// place output in the middle of window
			if (len > win_len){
				mvwprintw(win, row_count, col_offset, "long line ...");
			}else{
				mvwprintw(win, row_count,col_offset , buff);
			}
			row_count++;
		}
		wrefresh(win);
		if ( fclose(fr) == EOF){
			mvwprintw(win, 3,3, "error : cannot close file %s", CREDITS_FILE);
			wrefresh(win);
		}
	}

}


NSEW key1_to_NSEW(int key){		// TODO: one function key_to_NSEW(int key[player][key] ?
	if (key == keys_pl1[0]){	
		return NORTH;
	}else if (key == keys_pl1[1]){
		return SOUTH;
	}else if (key == keys_pl1[2]){
		return EAST;
	}else if (key == keys_pl1[3]){
		return WEST;
	}

	return -1; // to get rid of warrnings, normaly this wouldnt happen
}

int NSEW_to_key1(NSEW dir){
	switch(dir){
		case NORTH:	return keys_pl1[0];
					break;
		case SOUTH:	return keys_pl1[1];
					break;
		case EAST:	return keys_pl1[2];
					break;
		case WEST:	return keys_pl1[3];
					break;
	}
	return KEY_UP;
}

NSEW key2_to_NSEW(int key){		// TODO: one function key_to_NSEW(int key[player][key] ?
	if (key == keys_pl2[0]){	
		return NORTH;
	}else if (key == keys_pl2[1]){
		return SOUTH;
	}else if (key == keys_pl2[2]){
		return EAST;
	}else if (key == keys_pl2[3]){
		return WEST;
	}

	return -1; 	// to get rid of warrnings, normaly this wouldnt happen
}

int NSEW_to_key2(NSEW dir){
	switch(dir){
		case NORTH:	return keys_pl2[0];
					break;
		case SOUTH:	return keys_pl2[1];
					break;
		case EAST:	return keys_pl2[2];
					break;
		case WEST:	return keys_pl2[3];
					break;
	}
	return keys_pl2[0];
}


void wait_for_key(int how_long){
	sleep(how_long);

	flush_input();		// internaly calls nodelay(TRUE);
	nodelay(stdscr, FALSE);
	getch();
}
