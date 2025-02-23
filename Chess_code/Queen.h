#include "Piece.h"

class Queen : public Piece {
public:
	Queen(COLOR b_team, int b_row, int b_column, int b_count);
	Queen();
	bool can_classmove(int b_row, int b_column, Board* board_to_move_on);
};