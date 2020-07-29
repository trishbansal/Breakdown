#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pos.h"
#include "logic.h"
#include "board.h"

/*converts column character into an integer index*/
unsigned int char_to_int(char c) {
        int i = c;
        unsigned int col; 
        if(i >= 48 && i <= 57) {
                col = i - 48;
        } else if(i >= 65 && i <= 90) {
                col = i - 55;
        } else if(i >= 97 && i <= 122) {
                col = i - 61;
        } else {
		return -1;
	}
        return col;
}

/*switches which players turn it is*/
turn player(game* g) {
        turn curr = g->next;
        if(g->next == BLACK_NEXT) {
                curr = WHITE_NEXT;
        } else {
                curr = BLACK_NEXT;
        }
        return curr;
}

/*allows a player to drop a stick or make a breakdown legally in their turn*/
void make_move(game* g) {
	if(player(g) == BLACK_NEXT) {                                   
        	printf("White: ");                              
        } else if(player(g) == WHITE_NEXT) {                            
                printf("Black: ");                                      
        }  
        char ch;
        scanf("%c%*c", &ch);
        if(ch == '|') {
		printf("Column: ");
                char col;
                scanf("%c%*c", &col);
		if(char_to_int(col) == -1) {
			printf("invalid character entered: try again\n");
			make_move(g);
		} else if(vertically(g, char_to_int(col))) {
                } else {
                        make_move(g);
                }
        } else if(ch == '-') {
		printf("Column: ");
                char col;
                scanf("%c%*c", &col);
		if(char_to_int(col) == -1) {                                         
                        printf("invalid character entered: try again\n");                  
                        make_move(g);                                           
                }else if(horizontally(g, char_to_int(col))) {
                } else {
                        make_move(g);
                }
        } else if(ch == '!') {
                breakdown(g);
        } else {
		printf("invaid move: try again\n");
                make_move(g);
        }
}

/*switches between players, allowing players to make moves in their turn.
also returns game outcome when either a player wins or there is a draw*/
void play_game(game* g){
	while(game_outcome(g) == IN_PROGRESS) {
		board_show(g->b);
		g->next = player(g);
		make_move(g);
	}
	board_show(g->b);
	if(game_outcome(g) == BLACK_WIN) {
		printf("black win");
	} else if(game_outcome(g) == WHITE_WIN) {
		printf("white win");
	} else {
		printf("draw");
	} 	
}

/* run the game with given parameters */
int main(int argc, char *argv[]) {
	int i = 1; 
	unsigned int width = 0;
	unsigned int height = 0; 
	unsigned int stick = 0; 
	unsigned int square = 0;
	enum type rep = -1;
	unsigned int flag = 0; 
	if(argc != 10) {
		fprintf(stderr, "too many or too few command line arguments");
		exit(1);
	}
	while(i < argc) {
		if(strcmp(argv[i], "-w") == 0) {
			width = atoi(argv[i + 1]);
			i+=2;
		} else if(strcmp(argv[i], "-h") == 0) {
			height = atoi(argv[i + 1]);
			i+=2;
		} else if(strcmp(argv[i], "-k") == 0) {
			stick = atoi(argv[i + 1]);
			i+=2;
		} else if(strcmp(argv[i], "-q") == 0) {
			square = atoi(argv[i + 1]);
			i+=2;
		} else if(strcmp(argv[i], "-m") == 0) {
			if(flag == 0) {
				rep = MATRIX;
				i++;
				flag++;
			} else {
				fprintf(stderr, "type called multiple times");
				exit(1);
			}
		} else if(strcmp(argv[i], "-b") == 0) {
			if(flag == 0) {
				rep = BITS;
				i++;
				flag++;
			} else {
				fprintf(stderr, "type called multiple times");
                                exit(1);
			}
		}
	}
	if(height * width * stick * square == 0 || rep == -1) {
		fprintf(stderr, "one or more parameters not inputted");
		exit(1);	
	}
	if(stick > width || stick > height) {
		fprintf(stderr, "stick too long");
		exit(1);
	}
	unsigned int square_side; 
	if(height > width) {
		square_side = width;
	} else {
		square_side = height;
	}
	if(square > square_side) {
		fprintf(stderr, "winning square dimensions are out of bounds");
		exit(1);
	}
	game *g = new_game(stick, square, width, height, rep);
	play_game(g);
	game_free(g);	
	return 0;
}	
