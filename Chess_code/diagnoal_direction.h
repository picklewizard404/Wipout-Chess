#pragma once
#include "Piece.h"
enum DIAGONAL { UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT, WRONG };
struct DiagMove {
	DIAGONAL direction;
	int distance;
	DiagMove(DIAGONAL thedirection, int distance);
};
//Row and Column expected to be normal
void move_position_info_diag(int* row, int* column, DIAGONAL direction, int count);
DiagMove get_direction(Piece* tomove, int b_row, int b_column);