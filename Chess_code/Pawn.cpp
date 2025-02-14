#include "Pawn.h"
#include "Board.h"
#include <string.h>
#include "Move_safety.h"
#define _CRT_SECURE_NO_WARNINGS //prevents warnings for using non-Microsoft functions
#pragma warning(disable:4996)

//Aaron has helped and Andrew can still edit.

Pawn::Pawn() {
	be_safe(WHITE);
}

Pawn::Pawn(COLOR b_team, int b_row, int b_column, int b_count) {
	strcpy(chess_class, "Pawn");
	setup(chess_class, b_team, b_row, b_column, b_count, PAWN);
}

bool Pawn::can_classmove(int b_row, int b_column, Board* main_board) {
	if (!is_safe(this, main_board, b_row, b_column)) return false;
	int direction = 1;
	if (team == BLACK) {
		direction = -1;
	}
	if (row + direction != b_row) return false;
	if (b_column == column) {
		//Moving straight up or down.
		return !main_board->does_have_any_piece(b_row, b_column);
	}
	bool can_move_diagonaly = false;
	if ((b_column -1 <= column) && (column <= b_column + 1)) {
		can_move_diagonaly = main_board->does_have_any_piece(b_row, b_column) && main_board->no_ally_there(team, b_row, b_column);
	}
	
	return can_move_diagonaly;
}