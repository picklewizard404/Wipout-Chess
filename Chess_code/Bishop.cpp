
#include "Bishop.h"
#include "Board.h"
#include "Move_diagnolly.h"
#define _CRT_SECURE_NO_WARNINGS //prevents warnings for using non-Microsoft functions
#pragma warning(disable:4996)
Bishop::Bishop() {
	be_safe(WHITE);
}

void Bishop::setup(COLOR b_team, int b_row, int b_column, int b_count) {
	setspace(b_team, b_row, b_column, b_count);
}

Bishop::Bishop(COLOR b_team, int b_row, int b_column, int b_count) {
	setup(b_team, b_row, b_column, b_count);
}

bool Bishop::can_classmove(int b_row, int b_column, Board* board_to_move_on) {
	if (b_row == row || b_column == column) return false;
	DIAGONAL direction = UP_RIGHT;
	int movehoriz = 0, movevert = 0;
	if (b_row < row) {
		if (b_column < column) {
			direction = DOWN_LEFT;
			movehoriz = column - b_column;
		}
		else {
			direction = DOWN_RIGHT;
			movehoriz = b_column - column;
		}
		movevert = row - b_row;
	}
	else {
		if (b_column < column) {
			direction = UP_LEFT;
			movehoriz = column - b_column;
		}
		else {
			direction = UP_RIGHT;
			movehoriz = b_column - column;
		}
		movevert = b_row - row;
	}
	if (movevert != movehoriz) return false;
	return can_move_diagnolly(this, direction, movevert, board_to_move_on);
}