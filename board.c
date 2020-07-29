#include <stdio.h>
#include <stdlib.h> 
#include "board.h" 
#include <math.h>

board* board_new(unsigned int width, unsigned int height, enum type type) {
	if(type == BITS) {
		unsigned int* bit_b = (unsigned int*)
		calloc(ceil(((height * width) / 16.0)), sizeof(unsigned int));
		if(bit_b == NULL) {
			fprintf(stderr, "array allocation failed");
			exit(1);
		}
		board* b = (board*)malloc(sizeof(board));
		if(b == NULL) {
                        fprintf(stderr, "board allocation failed");
                        exit(1);
                }
		b->width = width;
                b->height = height;
                b->type = BITS;
                b->u.bits = bit_b;
		return b;
	} else { 
		cell** matrix_b = (cell**)malloc(sizeof(cell*) * height);
		if(matrix_b == NULL) {
			fprintf(stderr, "matrix allocation failed");
			exit(1);
		}	
		for(unsigned int r = 0; r < height; r++) {
			matrix_b[r] = (cell*)malloc(sizeof(cell) * width);
			if(matrix_b[r] == NULL) {
				fprintf(stderr, "matrix allocation failed");
				exit(1);
			}
			for(unsigned int c = 0; c < width; c++) {
				matrix_b[r][c] = EMPTY;
			}
		}
		board_rep b_rep = {matrix_b};
		board* b = (board*)malloc(sizeof(board));
		if(b == NULL) {
			fprintf(stderr, "board allocation failed");
			exit(1);
		}
		b->width = width;
		b->height = height;
		b->type = MATRIX;
		b->u = b_rep;
		return b;
	}
}

void board_free(board* b) {
	if(b->type == MATRIX) {
		for(int i = 0; i < b->height; i++) {
			free((b->u.matrix)[i]);
		}
		free(b->u.matrix);
		free(b);	
	} else {
		free(b->u.bits);
		free(b);
	}
}

/*decides what label to put on the axes*/
void label(unsigned int w_h) {
	if(w_h <= 9) {
		printf("%u", w_h);
        } else if(w_h <= 35) {
                char c = w_h + 55;
                printf("%c", c);
        } else if(w_h <= 61) {
                char c = w_h + 61;
                printf("%c", c);
        } else {
                printf("?");
        }
}



void board_show(board* b) {
	unsigned int i;
	unsigned int j;
	printf("  ");
	for(j = 0; j < b->width; j++) {
		label(j);
		printf(" ");
	}
	printf("\n");
	for(i = 0; i < b->height; i++) {
		label(i);
		printf(" ");
		for(j = 0; j < b->width; j++) {
			pos p = make_pos(i, j);
			if(board_get(b, p) == BLACK) {
				printf("* ");
			} else if (board_get(b, p) == WHITE) {
				printf("o ");
			} else {
				printf(". ");
			}
		}
		printf("\n");
	}
}
	  
cell board_get(board* b, pos p) {
	unsigned int width = b->width;
	unsigned int height = b->height; 
	if(width-1 < p.c && height-1 < p.r) {
                fprintf(stderr, "board_get failed: out of bounds\n");
                exit(1);
        }
	if(b->type == MATRIX) {
		cell got_cell = b->u.matrix[p.r][p.c];
		return got_cell;
	} else {
		unsigned int val_index = (p.r * width) + p.c; 
        	unsigned int array_index = floor(val_index / 16);
    		unsigned int cell_pos = (2 * (val_index % 16));
    		unsigned int bit = (b->u.bits[array_index] >> cell_pos) & 0x3;
		cell got_cell = bit;   
	/*	if (bit == 0x0) {
       			got_cell = EMPTY; 
    		} else if (bit == 0x1) {
        		got_cell = BLACK; 
    		} else if (bit == 0x2) {
        		got_cell = WHITE;
    		}*/
    		return got_cell;
	}
}

void board_set(board* b, pos p, cell c) {
	unsigned int width = b->width;
	unsigned int height = b->height;
	if(width-1 < p.c && height-1 < p.r) {
		fprintf(stderr, "board_set failed: out of bounds\n");
		exit(1);
	}
	if(b->type == MATRIX) {
		b->u.matrix[p.r][p.c] = c;
	} else {
		unsigned int val_index = (p.r * width) + p.c;
		unsigned int array_index = floor(val_index / 16);
		unsigned int cell_pos = (2 * (val_index % 16));
		unsigned int bit = b->u.bits[array_index];
		unsigned int to_empty = ~(0x3 << cell_pos); 
		bit = bit & to_empty; 
		bit = bit | c << cell_pos;
		b->u.bits[array_index] = bit;
	}
}
