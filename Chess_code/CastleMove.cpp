#include "CastleMove.h"
#include "InvalidPiece.h"

/* NOTE: ONLY CALLED IN Chess.cpp. 
*  Note that the constructor should throw an error every single time
   you try to make an invalid castle move!
   TODO: Write more tests to confirm castling is ONLY allowed when it should be, according to MY rules,
   which are slightly more generous because they allow kings to get along instead of killing each other.*/
CastleMove::CastleMove(Move kingmove, Rook* mrook_that_moved, CastleDirection mwhich_side_you_castled):
    Move(kingmove) {
    if (kingmove.piece_that_moved == NULL) {
        throw InvalidPiece(kingmove.piece_that_moved);
    }
    if (kingmove.piece_that_moved->piecetype != KING) {
        throw InvalidMove(std::string(kingmove.piece_that_moved->name) + std::string(" is NOT a king!"));;
    }
    if (kingmove.piece_that_moved->first_turn_i_moved() != -1) {
        throw InvalidMove("King moved already. Can't castle.");
    }
    if (mrook_that_moved->first_turn_i_moved() != -1) {
        throw InvalidMove("That rook moved already. Can't castle with that one!");
    }
    if (mwhich_side_you_castled == LEFT) {
        if (mrook_that_moved->column != 1) {
            throw InvalidMove("That rook isn't in the left column. Can't castle with that one!");
        }
    }
    else if (mwhich_side_you_castled == RIGHT) {
        if (mrook_that_moved->column != 8) {
            throw InvalidMove("That rook isn't in the right column. Can't castle with that one!");
        }
    }
    rook_that_moved = mrook_that_moved;
    //When the human moves, do this: rook_that_moved->know_i_moved(kingmove.piece_that_moved->first_turn_i_moved());
    which_side_you_castled = mwhich_side_you_castled;
}

CastleMove::CastleMove() {
    rook_that_moved = NULL;
    which_side_you_castled = LEFT;
}
