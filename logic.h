#ifndef LOGIC_H
#define LOGIC_H

#include "board.h"

/*holds the value of which players' turn it is*/
enum turn {
    BLACK_NEXT,
    WHITE_NEXT
};

typedef enum turn turn;

/*holds the outcome at a particular time*/ 
enum outcome {
    BLACK_WIN,
    WHITE_WIN,
    DRAW,
    IN_PROGRESS
};

typedef enum outcome outcome;

/*the structure of the game*/ 
struct game {
    unsigned int stick, square;
    board* b;
    turn next;
};

typedef struct game game;

/*creates a new game*/
game* new_game(unsigned int stick, unsigned int square, unsigned int width,
               unsigned int height, enum type type);

/*frees game*/
void game_free(game* g);

/*drops a stick in a particular column, vertically if veritcal is 1,
else horizontally*/
int drop_stick(game* g, unsigned int col, int vertical);

/*performs breakdown function such that all hovering pieces fall*/
void breakdown(game* g);

/*returns the outcome of the game at the time it is called*/
outcome game_outcome(game* g);

/*counts number of spaces from top available in a certain column*/
int spaces(game* g, unsigned int col);

/*returns highest row a horizontal stick can be dropped, starting at given col*/
int highest(game* g, unsigned int col);

/*tests whether squares form in board and returns the outcome*/
outcome square(game* g);

/*drops a vertical stick*/
int vertically(game* g, unsigned int col);

/*drops a horizontal stick*/
int horizontally(game* g, unsigned int col);

#endif /* LOGIC_H */
