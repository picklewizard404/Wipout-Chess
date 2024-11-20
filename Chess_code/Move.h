#include "Piece.h"
#ifndef DEFINE_MOVE
#define DEFINE_MOVE
struct Move
{
	int start_row;
	int start_column;
	int end_row;
	int end_column;
	Piece* piece_that_moved;
	Piece* piece_landed_on;
	Move();
	Move(int srow, int scolumn, int erow, int ecolumn, Piece* mpiece_that_moved, Piece* mpiece_landed_on);
};
#endif // !DEFINE_MOVE
