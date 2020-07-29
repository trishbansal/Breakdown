#ifndef BOARD_H
#define BOARD_H

#include "pos.h"

/*holds whether cell is empty, black, or white*/
enum cell {
    EMPTY,
    BLACK,
    WHITE
};

typedef enum cell cell;

union board_rep {
    enum cell** matrix;
    unsigned int* bits;
};

typedef union board_rep board_rep;

enum type {
    MATRIX, BITS
};

/*holds the structure of a board*/
struct board {
    unsigned int width, height;
    enum type type;
    board_rep u;
};

typedef struct board board;

/*creates a new board*/
board* board_new(unsigned int width, unsigned int height, enum type type);

/*frees a board*/
void board_free(board* b);

/*decides what label to put on the axes*/
void label(unsigned int w_h);

/*draws the current board*/
void board_show(board* b);

/*gets one particular cell of pos p from a board*/
cell board_get(board* b, pos p);

/*sets a certain pos on board to cell c*/ 
void board_set(board* b, pos p, cell c);

#endif /* BOARD_H */
