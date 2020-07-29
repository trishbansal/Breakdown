#ifndef POS_H
#define POS_H

/*structure for a position*/
struct pos {
    unsigned int r, c;
};

typedef struct pos pos;

/*makes a new position*/
pos make_pos(unsigned int r, unsigned int c);

#endif /* POS_H */
