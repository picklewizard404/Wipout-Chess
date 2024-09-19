#include "Piece.h"
#include "Board.h"
#include "diagnoal_direction.h"
//TODO Implement this function
#include <stdlib.h>
bool move_diagnolly(Piece* valid_piece, DIAGONAL direction, int count, Board* board_to_move_on) {
	if (!valid_piece->alive) return false;
	count = abs(count);
	int horizontal = count, vertical = count;
	//TODO Do this in calculate_diag_mv
	switch (direction) {
	case UP_LEFT:
		horizontal = -1 * horizontal;
		break;
	case UP_RIGHT:
		break;
	case DOWN_LEFT:
		horizontal = -1 * horizontal;
		vertical = -1 * vertical;
		break;
	case DOWN_RIGHT:
		vertical = -1 * vertical;
		break;
	}
	if (!(1 <= valid_piece->column + horizontal) <= 8) return false;
	if (!(1 <= valid_piece->row + vertical) <= 8) return false;

	//Check that  we aren't blocked
	int sp_horiz = valid_piece->row;
	int sp_vertic = valid_piece->column;
	for (int i = 1; i < count; i++)
	{
		//if(board_to_move_on[sp_horiz])
	}
	return false;
}