#pragma once
#ifndef KING_INCLUDED
#define KING_INCLUDED
#include "Piece.h"

class King : public Piece
{
public:
	King(COLOR b_team);
	King();
	bool can_kingmove(int b_row, int b_column, Board* main_board);
	void king_setup(COLOR b_team);
	bool am_in_check(Board* main_board);
	bool is_adjacent(int b_row, int b_column, bool test);
	bool can_classmove(int b_row, int b_column, Board* mainboard);
};
#endif