#include <stdio.h>
#include <stdlib.h>
#include "pos.h"
#include "board.h"
#include "logic.h"
//look at second outcomes for game outcome print functions
//was just a more efficient way to print and test 
//first one is inside an if statement
int main(int argc, char *argv[]) {
//testing label
	printf("expecting 2: ");
	label(2);
	printf("\n");
	printf("expecting A: ");
	label(10);
	printf("\n");
//testing board_new inside new_game
	game* g1 = new_game(4, 2, 50, 10, BITS); 
//testing board_show 
	printf("should show a board with 50 columns and 10 rows:\n");
	board_show(g1->b);
//testing make_pos, board_set, and board_show within it 
	pos p1 = make_pos(0, 4);
	board_set(g1->b, p1, BLACK);
	printf("should get a 50 by 10 board with pos(0,4) filled in black\n");
	board_show(g1->b);
//testing spaces, highest, vertically, horizontally, and drop_stick(& board get)
	drop_stick(g1, 3, 1);
	drop_stick(g1, 3, 0);
	drop_stick(g1, 3, 0);
	printf("4 spaces high vertical stick in column three");
	printf("2 horizontal sticks with len 4 hovering on it added\n");
	board_show(g1->b);
//testing breakdown 
	breakdown(g1);
	printf("horizontal sticks and pos(0,4) should fall to the last row\n");
	board_show(g1->b);
//testing square, no_drop, no_breakdown, and game_outcome
	printf("expecting: black win!: ");
	game_outcome(g1);
	printf("\n");
//testing multiple boards for game outcome (relies on all functions also)
	//1 
	game* g2 = new_game(2, 2, 4, 4, BITS);
	drop_stick(g2, 0, 1);
	g2->next = WHITE_NEXT;
	drop_stick(g2, 1, 1);
	g2->next = BLACK_NEXT;
	drop_stick(g2, 2, 1);
	g2->next = WHITE_NEXT;
	drop_stick(g2, 3, 1);
	board_show(g2->b);
	printf("4 x 4 board with 2 white and 2 black verticle sticks. ");
	printf("expecting: in progress : ");
	game_outcome(g2);
	printf("\n");
	drop_stick(g2, 3, 1);
	drop_stick(g2, 2, 1);
	drop_stick(g2, 1, 1);
	drop_stick(g2, 0, 1);
	board_show(g2->b);
	printf("4 x 4 board with 2 whole rows of white on top of previous. ");
	printf("expecting: white win! : ");
	game_outcome(g2);
	printf("\n");
	//2
	game* g3 = new_game(3, 3, 4, 4, BITS);
	drop_stick(g3, 0, 1);
	drop_stick(g3, 1, 1);
	g3->next = WHITE_NEXT;
	drop_stick(g3, 2, 1);
	drop_stick(g3, 3, 1);
	drop_stick(g3, 1, 0);
	board_show(g3->b);
	printf("4 x 4 board with 2 black verticle, 2 white vertical, and ");
	printf("one white horizantal stick. expecting: draw! : ");
	game_outcome(g3);
	printf("\n");
	game* g4 = new_game(6, 3, 15, 15, BITS);                                
        drop_stick(g4, 10, 1);                                                   
        drop_stick(g4, 11, 1);                                                   
        g4->next = WHITE_NEXT;                                                  
        drop_stick(g4, 8, 0);                                                   
        drop_stick(g4, 8, 0);                                                   
        drop_stick(g4, 6, 0);                                                
//	breakdown(g4);   
        board_show(g4->b);                                                      
        printf("15 x 15 board with 2 black vertical sticks and 3 white horz. ");
	printf("expecting: white win! : ");                                          
        game_outcome(g4);                                                       
        printf("\n"); 	
	return 0;
}	
