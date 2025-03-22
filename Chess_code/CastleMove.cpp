#include "CastleMove.h"
#include "InvalidPiece.h"

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
	rook_that_moved = mrook_that_moved;
	//When the human moves, do this: rook_that_moved->know_i_moved(kingmove.piece_that_moved->first_turn_i_moved());
	which_side_you_castled = mwhich_side_you_castled;
}
