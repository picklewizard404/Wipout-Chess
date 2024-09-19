#pragma once
#include "Piece.h"
#include "Team.h"
#include <stdio.h>

typedef Piece * space;

class Board
{
	//variables
public:
	// Piece **spaces[8][8];
	Piece* spaces[8][8];
	Piece* threatens_white;
	Piece* threatens_black;

	// int column;
	// int row;
	bool whiteturn;

	//functions move
	bool move(space piece, int b_row, int b_column, bool* landed_on_king = NULL);
	void place(Piece* piece, int row, int column);
	void print_board();
	bool is_on_board(int b_row, int b_column);
	Board();
	bool is_in_check(King* king_of_team_1, Team* team_2, Board* mainboard);
	bool does_have_any_piece(int row, int column);
	bool can_fight(COLOR my_team, int row, int column);
};

//bool do_team_dismatch(Piece* team_moving, Piece* team_there)