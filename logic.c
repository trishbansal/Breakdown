#include <stdio.h>
#include <stdlib.h>
#include "logic.h"
#include "board.h"
#include "pos.h"

game* new_game(unsigned int stick, unsigned int square, unsigned int width, 
	       unsigned int height, enum type type) {
	game* g = (game*)malloc(sizeof(game));
	if(g == NULL) {
		fprintf(stderr, "game allocation failed");
		exit(1);
	}
	board* gameboard = board_new(width, height, type);
	g->stick = stick;
	g->square = square;
	g->b = gameboard;
	g->next = BLACK_NEXT;
	return g;
}

void game_free(game* g){
	board_free(g->b);
	free(g);
}

//DROP STICK
/*counts number of spaces available in a certain col */
int spaces(game* g, unsigned int col) {
	unsigned int curr_row = 0;
	unsigned int tot_row = g->b->height;
	int empty_rows = 0;
	while(curr_row < tot_row) {
		pos curr_pos = make_pos(curr_row, col);
		cell curr_cell = board_get(g->b, curr_pos);
		if(curr_cell == EMPTY) {
			//printf("empty\n");
			empty_rows++;
			curr_row++;
		} else {
			return empty_rows - 1;
		}
	}
	return empty_rows - 1;
}

/*drops a vertical stick*/
int vertically(game* g, unsigned int col) {
	unsigned int place = spaces(g, col);
	unsigned int stick_len = g->stick;
	if(place + 1 < stick_len) {
		return 0;
	} 
	unsigned int curr_row;
	for(curr_row = 0; curr_row < stick_len; curr_row++) {
		pos p = make_pos(place - curr_row, col);
		cell c = EMPTY;
		if(g->next == BLACK_NEXT) {
			c = BLACK;
		} else {
			c = WHITE;
		}
		board_set(g->b, p, c);
	}
	return 1;
}

/*finds highest empty row starting at col till length of stick*/
int highest(game* g, unsigned int col) {
	int curr_row = g->b->height - 1;
	unsigned int stick_len = g->stick;
	int highest_row = g->b->height - 1;
	while(curr_row >= 0) {
		unsigned int curr_col;
		for(curr_col = col; (curr_col - col) < stick_len; curr_col++) {
			pos p = make_pos(curr_row, curr_col);
			if(board_get(g->b, p) != EMPTY) {
				highest_row = curr_row - 1;
				continue;
			}	 
		}
		curr_row--;
	}
	return highest_row;
}

/*drops a horizontal stick*/    
int horizontally(game* g, unsigned int col) {
	unsigned int stick_len = g-> stick; 
	if(col + stick_len > g->b->width) {
		return 0;
	} else {
		unsigned int curr_col;
		int curr_row = highest(g, col);
		if(curr_row < 0) {
			return 0;
		} 
		for(curr_col = col; (curr_col - col) < stick_len; curr_col++) {
			pos p = make_pos(curr_row, curr_col);
			cell c = EMPTY;
                        if(g->next == BLACK_NEXT) {
                                c = BLACK;
                        } else {
                                c = WHITE;
                        }
			board_set(g->b, p, c);
		}
	}
	return 1;
}

int drop_stick(game* g, unsigned int col, int vertical){
	if(vertical == 1){
		return vertically(g, col);
	} else {
		return horizontally(g, col);
	}
}

//BREAKDOWN
void breakdown(game* g) {
	unsigned int last_row = g->b->height - 1;
	unsigned int width = g->b->width;
	unsigned int pass = 0;
	unsigned int row_count;
	unsigned int col_count;
	for(row_count = last_row; row_count > 0; row_count--) {
		for(col_count = 0; col_count < width; col_count++) {
			unsigned int row_above = row_count - 1;
			pos p1 = make_pos(row_count, col_count);
			pos p2 = make_pos(row_above, col_count);
			if(board_get(g->b, p1) == EMPTY && 
			   board_get(g->b, p2) == BLACK) {
				board_set(g->b, p2, EMPTY);
				board_set(g->b, p1, BLACK);
			} else if(board_get(g->b, p1) == EMPTY && 
				  board_get(g->b, p2) == WHITE) {
				board_set(g->b, p1, WHITE);
				board_set(g->b, p2, EMPTY);
			} else {
				pass++;
			}
		}
	}
	if(pass != last_row * width) {
		breakdown(g);
	} else {
		return;
	}
}

//GAME OUTCOME
/*tests whether there is a winning square or multiple squares*/
outcome square(game* g) {
	unsigned int sidelen = g->square;
	unsigned int height = g->b->height;
	unsigned int width = g->b->width;
	unsigned int row_count;
	unsigned int col_count;
	unsigned int row_sl;
	unsigned int col_sl;
	unsigned int black_sq = 0;
	unsigned int white_sq = 0;
	for(row_count = 0; row_count <= height - sidelen; row_count++) {
		for(col_count = 0; col_count <= width - sidelen; col_count++) {
			unsigned int black = 0;
			unsigned int white = 0;
			for(row_sl = 0; row_sl < sidelen; row_sl++) {
				for(col_sl = 0; col_sl < sidelen; col_sl++) {
					pos p = make_pos(row_sl + row_count, 
							 col_sl+ col_count);
					if(board_get(g->b, p) == BLACK) {
						black++;
					} else if(board_get(g->b, p) == WHITE) {
						white++;
					}
				}
			}
        		if(black == (sidelen * sidelen)) {
                		black_sq++;
        		}  else if (white == (sidelen * sidelen)) {
                		white_sq++;
        		}
		}
	}
	if(black_sq > 0 && white_sq > 0) {
		return DRAW;
	} else if(black_sq > 0) {
		return BLACK_WIN;
	} else if(white_sq > 0) {
		return WHITE_WIN;
	} else {
		return IN_PROGRESS;
	}
}

/*tests whether it is possible to drop a stick in the current board*/
int no_drop(game* g) {
	unsigned int curr_col;
        unsigned int counter = 0;
        for(curr_col = 0; curr_col < g->b->width; curr_col++) {
               	if(highest(g, curr_col) >= 0 || 
		   spaces(g, curr_col) + 1 > g->stick) {
                        counter++;
                }
        }
        if(counter == 0) {
                return 1;
        } else {
                return 0;
        }
}

/*tests whether it is possible to have a breakdown in the current board*/
int no_breakdown(game* g) {
	game* g2 = new_game(g->stick, g->square, g->b->width, 
			    g->b->height, g->b->type);
	for(unsigned int i = 0; i < g->b->height; i++) {
		for(unsigned int j = 0; j < g->b->width; j++) {
			pos p = make_pos(i, j);
			cell val = board_get(g->b, p);
			board_set(g2->b, p, val);
		}
	}
	breakdown(g2);
	unsigned int curr_row;
	unsigned int curr_col;
	unsigned int num_diff = 0;
	for(curr_row = 0; curr_row < g->b->height; curr_row++) {
		for(curr_col = 0; curr_col < g->b->width; curr_col++){
			pos p1 = make_pos(curr_row, curr_col);
			if(board_get(g->b, p1) != board_get(g2->b, p1)) {
				num_diff++;
			}
		}
	}
	if(num_diff > 0) {
		return 0;
	} else {
		return 1;
	}
}

	
outcome game_outcome(game* g) {
	if(square(g) != IN_PROGRESS) {
		return square(g);
	} else if(no_breakdown(g) && no_drop(g)) { 
		return DRAW;
	} else {
		return square(g);
	} 	
}
