#pragma once
#include "Teamname.h"
class Board;
enum TYPE { PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING, EMPTY };
class Piece
{
public: // public variables
    //The longest name is 9 characters and the last one is \0.
    COLOR team;
    char chess_class[8];
    //Important. Length of name.
    char name[10];
    //Important. This decides whether or not the piece should stay on the board.
    bool alive;
    int count;
    int row;
    int column;
    TYPE piecetype;


public: // public functions
    /*b_row and b_column range from 1 to 8 because they are displayed to the user.*/
    Piece();
    Piece(COLOR color);
    Piece(Piece* clone);
    char team_character();
    void setup(char* typeofpiece, COLOR b_team, int b_row, int b_column, int b_count, TYPE b_piecetype);
    //b_row and b_column are 1 at minimun
    virtual bool can_classmove(int b_row, int b_column, Board *board_to_move_on);
    bool do_team_match(Piece* team_there);
    void be_safe(COLOR color);
    // void can_move(Board *pBoard);
    // void setupnullpiece();
    void sayspace();
    void know_i_change_position(int b_row, int b_column);
   // virtual void setup(char b_team, int b_column, int b_row, int b_count);
    void setspace(COLOR b_team, int b_row, int b_column, int b_count);
    void set_up_full_name(const char* nameofpiece);
    bool is_on_board(int b_row, int b_column);
    bool same_team(char piece_team);
    Piece clone();
    void place(int b_row, int b_column);
};

