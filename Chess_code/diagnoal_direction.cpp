#include "diagnoal_direction.h"
#include "Piece.h"

//Count should be positive!
void move_position_info_diag(int* row, int* column, DIAGONAL direction, int count) {
    switch (direction) {
    case DIAGONAL::UP_LEFT:
        *row = count + *row;
        *column = (-1 * count) + *column;
        break;
    case DIAGONAL::UP_RIGHT:
        *row = count + *row;
        *column = count + *column;
        break;
    case DIAGONAL::DOWN_LEFT:
        *row = (-1 * count) + *row;
        *column = ( - 1 * count) + *column;
        break;
    case DIAGONAL::DOWN_RIGHT:
        *row = (-1 * count) + *row;
        *column = count + *column;
        break;
    }
}

DiagMove::DiagMove(DIAGONAL thedirection, int thedistance)
{
    direction = thedirection;
    distance = thedistance;
}


//Row and Column expected to be normal
DiagMove get_direction(Piece* tomove, int b_row, int b_column) {
    if (b_row == tomove->row || b_column == tomove->column) return DiagMove(DIAGONAL::WRONG, 0);
    DIAGONAL direction = DIAGONAL::UP_RIGHT;
    int movehoriz = 0, movevert = 0;
    if (b_row < tomove->row) {
        if (b_column < tomove->column) {
            direction = DIAGONAL::DOWN_LEFT;
            movehoriz = tomove->column - b_column;
        }
        else {
            direction = DIAGONAL::DOWN_RIGHT;
            movehoriz = b_column - tomove->column;
        }
        movevert = tomove->row - b_row;
    }
    else {
        if (b_column < tomove->column) {
            direction = DIAGONAL::UP_LEFT;
            movehoriz = tomove->column - b_column;
        }
        else {
            direction = DIAGONAL::UP_RIGHT;
            movehoriz = b_column - tomove->column;
        }
        movevert = b_row - tomove->row;
    }
    if (movevert != movehoriz) return DiagMove(DIAGONAL::WRONG, 0);
    return DiagMove(direction, movevert);
}