#include "Move.h"
#include "Board.h"
#include "Team.h"
//TODO: Re-write this function in Board.cpp. Just copy and paste.
//Leave the note
//UNDO_MOVE8
//OVER EACH DEFINITION.
void undo_move(Move* tried_move, Board* mainboard, Team* team_undoing_move, Game_Status* game_status) {
	mainboard->place(tried_move->piece_that_moved, tried_move->start_row, tried_move->start_column);
	tried_move->piece_that_moved->row = tried_move->start_row;
	tried_move->piece_that_moved->column = tried_move->start_column;
	if (tried_move->piece_landed_on != NULL) {
		tried_move->piece_landed_on->alive = true;
		mainboard->place(tried_move->piece_landed_on, tried_move->end_row, tried_move->end_column);
		*game_status = mainboard->is_in_check(team_undoing_move->enemy_team, team_undoing_move, mainboard, false);
	}
}