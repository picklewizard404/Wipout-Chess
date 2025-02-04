#include "Move.h"
#include "Board.h"
#include <string>
//If you call this, you have to set every single variable.
InvalidMove::InvalidMove(int b_row, int b_column) {
    row = b_row;
    column = b_column;
    std::string theproblem = "You tried to hit ";
    theproblem += "end row ";
    theproblem += std::to_string(row);
    theproblem += " end column ";
    theproblem += std::to_string(column);
    theproblem += " with nothing.";
    problem = theproblem + "\n";
}
//Credit: https://en.cppreference.com/w/cpp/error/exception/what
Move::Move() {
    start_row = 0;
    start_column = 0;
    end_row = 0;
    end_column = 0;
    piece_that_moved = NULL;
    piece_landed_on = NULL;
}
Move::Move(int srow, int scolumn, int erow, int ecolumn, Piece* mpiece_that_moved, Piece* mpiece_landed_on) {
    printf("Piece that moved: %s\n", mpiece_that_moved == NULL ? "Oops" : mpiece_that_moved->name);
    if (mpiece_that_moved == NULL) {
        printf("A bad move was made.\n");
        throw InvalidMove(erow, ecolumn);
    }
    start_row = srow;
    end_row = erow;
    start_column = scolumn;
    end_column = ecolumn;
    piece_that_moved = mpiece_that_moved;
    piece_landed_on = mpiece_landed_on;
}