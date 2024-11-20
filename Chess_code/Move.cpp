#include "Move.h"
#include "Board.h"
//If you call this, you have to set every single variable.
Move::Move() {
    start_row = 0;
    start_column = 0;
    end_row = 0;
    end_column = 0;
    piece_that_moved = NULL;
    piece_landed_on = NULL;
}
Move::Move(int srow, int scolumn, int erow, int ecolumn, Piece* mpiece_that_moved, Piece* mpiece_landed_on) {
    start_row = srow;
    end_row = erow;
    start_column = scolumn;
    end_column = ecolumn;
    piece_that_moved = mpiece_that_moved;
    piece_landed_on = mpiece_landed_on;
}