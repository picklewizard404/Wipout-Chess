#ifndef CASTLE_MOVE
#define CASTLE_MOVE

#include "Move.h"
#include "Rook.h"
#include "Board.h"
enum CastleDirection {
    LEFT,
    RIGHT
};
struct CastleMove: public Move
{
    Rook* rook_that_moved;
    CastleDirection which_side_you_castled;
    CastleMove(Move kingmove, Rook* mrook_that_moved, CastleDirection mwhich_side_you_castled, Board *board_its_on, Team* myteam);
    CastleMove();
};
#endif // !CASTLE_MOVE
