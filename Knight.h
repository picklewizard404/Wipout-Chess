//#pragma once
/*
PRAGMA ONCE basically does the same thing as
#ifndef whatever
Contents...
#endif //whatever
Re-writning the public compile variable helps you remember it.
*/
#ifndef KNIGHT_INCLUDED
#define KNIGHT_INCLUDED
#include "Piece.h"
#include "Teamname.h"

class Board;

class Knight : public Piece
{
public:
    Knight(COLOR b_team, int b_row, int b_column, int b_count);
    Knight();
    bool can_classmove(int b_row, int b_column, Board *mainboard);
  //  virtual void setup(COLOR b_team, int b_row, int b_column, int b_count);
};
#endif //KNIGHT-INCLUDED
