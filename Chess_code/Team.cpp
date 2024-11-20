#include "Knight.h"
#include "King.h"
#include "Team.h"
#include "Teamname.h"
#include "Piece.h"
#include "Board.h"
#include "Rook.h"
#include "Check_vs_Checkmate.h"
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS //prevents warnings for using non-Microsoft functions
#pragma warning(disable:4996)
//public
//variables


Team::Team() {
	enemy_team = nullptr;
	empty_spaces();
}
Team::Team(COLOR team_color, Board *the_board_shared) :the_king(WHITE)
{
	enemy_team = nullptr;
	empty_spaces();
	color = team_color;
	current_status = NEUTRAL;
	if (team_color == WHITE) {
		namefirstletter = 'w';
		strcpy(full_name, "White");
		empty_spaces();
		knight1 = Knight(team_color, 1, 2, 1);
		knight2 = Knight(team_color, 1, 7, 2);
		rook1 = Rook(team_color, 1, 1, 1);
		rook2 = Rook(team_color, 1, 8, 2);
	}
	else //team_color == BLACK
	{
		namefirstletter = 'b';
		strcpy(full_name, "Black");
		empty_spaces();
		knight1 = Knight(team_color, 8, 2, 1);
		knight2 = Knight(team_color, 8, 7, 2);
		rook1 = Rook(team_color, 8, 1, 1);
		rook2 = Rook(team_color, 8, 8, 2);
	}
	pieces[0] = &rook1;
	pieces[7] = &rook2;
	pieces[1] = &knight1;
	pieces[6] = &knight2;
	the_king = King(team_color);
	pieces[4] = &the_king;
	// Now put the pieces on our team on the board.
	put_pieces_on_board(the_board_shared);
}
void Team::empty_spaces() {
	for (int i = 0; i < 16; i++) {
		pieces[i] = 0;
	}
}

//TODO: Function to change a piece's class.
void Team::put_pieces_on_board(Board* the_board_shared) {
	for (int i = 0; i < 16; i++) {
		if (pieces[i] != 0) {
			the_board_shared->spaces[pieces[i]->row - 1][pieces[i]->column - 1] = pieces[i];
		}
	}
}