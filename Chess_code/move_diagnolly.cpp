#include "Piece.h"
#include "Board.h"
#include "diagnoal_direction.h"
#include <stdlib.h>
bool can_move_diagnolly(Piece* valid_piece, DIAGONAL direction, int count, Board* board_to_move_on) {
    if (!valid_piece->alive) return false;
    if (direction == WRONG) return false;
    count = abs(count);
    int horizontal_direction = 1, vertical_direction = 1;
    int berow = valid_piece->row;
    int becolumn = valid_piece->column;
    move_position_info_diag(&berow, &becolumn, direction, count);
    if (!(1 <= berow && berow <= 8)) return false;
    if (!(1 <= becolumn && becolumn <= 8)) return false;

    //Check that  we aren't blocked
    //Row changes vertically.
    int sp_vertic = valid_piece->row;
    //Column changes horizontally.
    int sp_horiz = valid_piece->column;
    if (direction == DOWN_LEFT || direction == DOWN_RIGHT) {
        vertical_direction = -1;
    }
    if (direction == UP_LEFT || direction == DOWN_LEFT) {
        horizontal_direction = -1;
    }

    for (int i = 1; i < count; i++)
    {
        if ((*board_to_move_on).spaces[sp_vertic + i*vertical_direction - 1][sp_horiz + i*horizontal_direction - 1] != NULL) {
            return false;
        }
    }
    if ((*board_to_move_on).spaces[sp_vertic + count * vertical_direction - 1][sp_horiz + count * horizontal_direction - 1] == NULL) return true;
    return !(*board_to_move_on).spaces[sp_vertic + count * vertical_direction - 1][sp_horiz + count * horizontal_direction - 1]->same_team(valid_piece->team); //
}