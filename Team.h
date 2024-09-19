#pragma once
#ifndef TEAM_INCLUDED
#define TEAM_INCLUDED
#include "Knight.h"
#include "King.h"
#include "Teamname.h"
#include "Piece.h"
#include "Rook.h"
#include "Pawn.h"

class Board;

class Team {
	//public
public:
	//variables
	char namefirstletter;
	char full_name[6];
	Piece* pieces[16];
	COLOR color;
	Knight knight1;
	Knight knight2;
	Rook rook1;
	Rook rook2;
	King the_king;
	Team* enemy_team;

	//functions
	//Team(COLOR team_color);
	Team(COLOR team_color, Board* the_board_shared);
	Team();
	void empty_spaces();
	void upgrade_pawn(Pawn* pawn_to_upgrade);
	void put_pieces_on_board(Board* the_board_shared);
};

#endif TEAM_INCLUDED