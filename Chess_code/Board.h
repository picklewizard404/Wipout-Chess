#pragma once
#include "Piece.h"
#include "Team.h"
#include <stdio.h>
#include "Check_vs_Checkmate.h"
#include "Move.h"

typedef Piece * space;
class Board
{
	//variables
public:
	Piece* spaces[8][8];
	Piece* threatens_white;
	Piece* threatens_black;
	Pawn* pawnthatjustmoved2;
	int passant_row;
	int passant_column;

	// int column;
	// int row;
	bool whiteturn;

	//functions move
	void clearpassant();
	bool human_move_piece(Move* move_to_make);
	void place(Piece* piece, int row, int column);
	void kill_passant();
	void print_board();
	bool is_on_board(int b_row, int b_column);
	Board();
	Game_Status is_in_check(Team* my_team, Team* enemy_team, Board* mainboardbool, bool check_for_checkmate = true);
	Game_Status try_to_escape(Team* my_team, Team* enemy_team, Board* mainboard);
	bool does_have_any_piece(int row, int column);
	bool no_ally_there(COLOR my_team, int row, int column);
};

//bool do_team_dismatch(Piece* team_moving, Piece* team_there)