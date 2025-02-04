#pragma once
#include "Piece.h"
#include "Board.h"
#include "diagnoal_direction.h"

//Test string
bool can_move_diagnolly(Piece* valid_piece, DIAGONAL direction, int count, Board* board_to_move_on);
