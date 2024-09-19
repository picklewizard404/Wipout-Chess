#include "Rook.h"
#include "Board.h"
#include "Move_horizontally.h"
#include <string.h>

Rook::Rook() {
    be_safe(WHITE);
}

Rook::Rook(COLOR b_team, int b_row, int b_column, int b_count) {
	setup(b_team, b_row, b_column, b_count);
}

void Rook::setup(COLOR b_team, int b_row, int b_column, int b_count) {
    be_safe(b_team);
    strcpy_s(chess_class, "Rook");
    piecetype = ROOK;
    setspace(b_team, b_row, b_column, b_count);
    //Do name it completly correctly
    set_up_full_name(chess_class);
}

//The letter b means I'm gonna BE here.
bool Rook::can_classmove(int b_row, int b_column, Board* board_to_move_on) {
    return move_horizontally(this, b_row, b_column, board_to_move_on);
}