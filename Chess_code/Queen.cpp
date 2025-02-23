#pragma once
#include "Queen.h"
#include "move_diagnolly.h"
#include "move_horizontally.h"
//Reminder: Always say the row BEFORE the column.
bool Queen::can_classmove(int b_row, int b_column, Board* board_to_move_on) {
	bool can_move = false;
	DiagMove diagmove = get_direction(this, b_row, b_column);
	can_move &= can_move_diagnolly(this, diagmove.direction, diagmove.distance, board_to_move_on);
	return can_move;
}