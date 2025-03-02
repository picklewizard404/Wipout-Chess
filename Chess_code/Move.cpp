#include "Move.h"
#include "Board.h"
#include <string>
#include "InvalidPiece.h"
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
    new_passant_if_any = PassantPawn();
}
void Move::print_move() {
    if (piece_that_moved == NULL) return;
    printf("Moved %s to row %d, column %d.\n", piece_that_moved->name, end_row, end_column);
}
Move::Move(int srow, int scolumn, int erow, int ecolumn, Piece* mpiece_that_moved, Piece* mpiece_landed_on, PassantPawn* spassant_if_any, PassantPawn* mpassant_if_any, bool sayimoved) {
    if (sayimoved) {
        print_move();
    }
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
    new_passant_if_any = PassantPawn(*mpassant_if_any);
}