#pragma once
#include "Piece.h"
#include "Teamname.h"

class Board;

class Pawn : public Piece
{
private:
    int starting_column;
public:
    int get_start_column();
    Pawn(COLOR b_team, int b_row, int b_column, int b_count);
    Pawn();
    bool can_classmove(int b_row, int b_column, Board* mainboard);
    //  virtual void setup(COLOR b_team, int b_row, int b_column, int b_count);
};
