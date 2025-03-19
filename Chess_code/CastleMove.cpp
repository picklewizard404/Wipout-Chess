#include "CastleMove.h"
#include "InvalidPiece.h"

CastleMove::CastleMove(Move kingmove, Rook* mrook_that_moved, CastleDirection mwhich_side_you_castled) : 
	Move(kingmove) {
	if (kingmove.piece_that_moved == NULL) {
		throw InvalidPiece(kingmove.piece_that_moved);
	}
	rook_that_moved = mrook_that_moved;
	which_side_you_castled = mwhich_side_you_castled;
}
