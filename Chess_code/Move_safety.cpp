#include "Piece.h"
#include "Board.h"
bool is_safe(Piece* piece_to_move, Board* main_board, int b_row, int b_column) {
    if ((b_row < 1) || (b_row > 8) || (b_column < 1) || (b_column > 8)) {
        return false;
    }
    if (!(piece_to_move->alive)) return false;
    Piece* move_to = main_board->spaces[b_row - 1][b_column - 1];
    if (move_to == NULL) return true;
    if (main_board->spaces[b_row - 1][b_column - 1]->team == piece_to_move->team) return false;
    return true;
}