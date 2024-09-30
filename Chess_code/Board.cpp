#include "Piece.h"
#include "Knight.h"
#include "Board.h"

#include <iostream>
#include "Safety.h"
#include "Check_vs_Checkmate.h"
#include "Move.h"
using namespace std;

Board::Board() {
    whiteturn = true;
    threatens_white = NULL;
    threatens_black = NULL;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            spaces[i][j] = NULL;
        }
    }
}
//Column and row both range from 1 to 8.
//The piece has to know where it was when this move happens
void Board::place(Piece* piece, int row, int column) {
    spaces[piece->row - 1][piece->column - 1] = NULL;
    spaces[row - 1][column - 1] = piece;
    piece->row = row;
    piece->column = column;
}

bool Board::does_have_any_piece(int row, int column) {
    return spaces[row - 1][column - 1] != NULL;
}

bool Board::can_fight(COLOR my_team, int row, int column) {
    if (spaces[row - 1][column - 1] != NULL) {
        return spaces[row - 1][column - 1]->team != my_team;
    }
    else {
        return true;
    }
}

//TODO pass both teams. Check where this function is called and make sure teams are passed.
//Maybe I am calling this function for the wrong team?
//The team we are checking check for is team 1.
Game_Status Board::is_in_check(Team* my_team, Team* enemy_team, Board* mainboard, bool check_for_checkmate)  {
    //Todo: Use info HERE to try every possible move of my_team
    int column = my_team->the_king.column;
    int row = my_team->the_king.row;
    bool is_hugging_allowed = false;
    for (int i = 0; i < 16; i++) {
        if (enemy_team->pieces[i] != 0) {
            switch (enemy_team->pieces[i]->piecetype) {
            case KING:
                if ((King*)enemy_team->pieces[i]->can_classmove(row, column, mainboard)) {
                    if (check_for_checkmate) {
                        return try_to_escape(my_team, enemy_team, mainboard);
                    }
                    else return CHECK;
                }
            case KNIGHT:
                if ((Knight*)enemy_team->pieces[i]->can_classmove(row, column, this)) {
                    if (check_for_checkmate) {
                        return try_to_escape(my_team, enemy_team, mainboard);
                    }
                    else return CHECK;
                }
            case ROOK:
                if ((Rook*)enemy_team->pieces[i]->can_classmove(row, column, this)) {
                    if (check_for_checkmate) {
                        return try_to_escape(my_team, enemy_team, mainboard);
                    }
                    else return CHECK;
                }
                //Empty spaces go here and do not threaten you
            default:
                continue;
            }
        }
    }
    return NEUTRAL;
}


Game_Status Board::try_to_escape(Team* my_team, Team* enemy_team, Board* mainboard) {
    Piece* one_of_my_pieces;
    Move tried_move;
    Game_Status current_status = CHECK;
    //I don't really care about hugging so I will assume I didn't land on a king;
    bool landed_on_a_king = false;
    for (int i = 0; i < 16; i++) {
        one_of_my_pieces = my_team->pieces[i];
        if (my_team->pieces[i] == NULL) {
            continue;
        }
        if (one_of_my_pieces->alive == false) {
            continue;
        }
        tried_move.start_row = one_of_my_pieces->row;
        tried_move.start_column = one_of_my_pieces->column;
        tried_move.piece_that_moved = one_of_my_pieces;
        for (int row = 1; row <= 8; row++) {
            for (int column = 1; column <= 8; column++) {
                if (one_of_my_pieces->can_classmove(row, column, mainboard)) {
                    tried_move.end_row = row;
                    tried_move.end_column = column;
                    tried_move.piece_landed_on = mainboard->spaces[row - 1][column - 1];
                    mainboard->move_piece(&tried_move);
                    //TODO: Check if you are still in check. If you are, the loop continues.
                    //If not, return CHECK because we know we can be safe again.
                    //Either way the move must be un_done.
                    if (mainboard->is_in_check(my_team, enemy_team, mainboard, false) == NEUTRAL) {
                        //TODO: Undo the move.
                        Move undomove = Move(
                            tried_move.end_row,
                            tried_move.end_column,
                            tried_move.start_row,
                            tried_move.start_column,
                            tried_move.piece_that_moved,
                            NULL
                        );
                        mainboard->move_piece(&undomove);
                        one_of_my_pieces->row = tried_move.start_row;
                        one_of_my_pieces->column = tried_move.start_column;
                        if (tried_move.piece_landed_on != NULL) {
                            tried_move.piece_landed_on->alive = true;
                            mainboard->place(tried_move.piece_landed_on, row, column);
                        }
                        return CHECK;
                    }
                    else {
                        //TODO Undo the move
                    }
                }
            }
        }
    }
    //If this loop ends, you can't escape Check.
    return CHECKMATE;

};

bool Board::is_on_board(int b_row, int b_column) {
    if (b_row < 1 || b_column < 1 || b_row > 8 || b_column > 8) {
        return false;
    }
    return true;
}
/*
b_column and b_row range from 1 to 8. We subtract 1 whenever we need to hit a space on the board.
That's because a person starts counting spaces with 1 but the computer starts counting with 0.
Do this whenever you touch a space on the board.
*/
bool Board::move_piece(Move* move_to_make) {
    space piece = move_to_make->piece_that_moved;
    int b_row = move_to_make->end_row;
    int b_column = move_to_make->end_column;
    if (piece != NULL) {
        int p_column = piece->column;
        int p_row = piece->row;
        bool can_move = piece->can_classmove(b_row, b_column, this);
        if (piece != NULL) {
            if (piece->piecetype == KING) {
                can_move = can_move || ((King*)piece)->can_kingmove(b_row, b_column, this);
            }
        }
        if (can_move) {
            // If you landed on a piece on your team:
            if (piece->do_team_match(spaces[b_row - 1][b_column - 1])) {
                return false;
            }

            //Opposing team:
            if (spaces[b_row - 1][b_column - 1] != NULL) {
                spaces[b_row - 1][b_column - 1]->alive = false;
                spaces[b_row - 1][b_column - 1] = piece;
                //Remember the fact that I was there in case this move has to be redone.
                move_to_make->piece_landed_on = spaces[b_row - 1][b_column - 1];
                //TODO: Add this to the stack of moves.
            }



            //Either way move the piece.

            place(piece, b_row, b_column);
            piece->know_i_change_position(b_row, b_column);
            return true;
        }
        return false;
    }
    return false;
}
void Board::print_board() {
    int length_of_name = 10;
    int number_of_spaces = 8;
    bool end_found = false;
    bool is_alive = true;
    for (int p_row = 7; p_row > -1; p_row--) {
        for (int p_column = 0; p_column < number_of_spaces; p_column++) {
            if (spaces[p_row][p_column] != NULL) {
                is_alive = spaces[p_row][p_column]->alive;
            }
            if ((p_row == 7 || p_row == 0) and (p_column == 0)) {
//                if (spaces[p_row][p_column] == NULL)
                if ((spaces[p_row][p_column] == NULL) || !is_alive)
                {
                    printf("%-11s|", "[]");
                    continue;
                }
                else {
                    printf("[]%-9s|", spaces[p_row][p_column]->name);
                }
            }
            else if (((p_row < 7) && (p_row > 0)) || ((p_column < 7) && (p_column > 0))) {
//                if (spaces[p_row][p_column] == NULL)
                if (spaces[p_row][p_column] == NULL || !is_alive)
                {
                    printf("%-11s|", "");
                    continue;
                }
                else {
                    printf("%-11s|", spaces[p_row][p_column]->name);
                }
                
            }
            if ((p_row == 7 || p_row == 0) and (p_column == 7)) {
                if (spaces[p_row][p_column] != NULL || !is_alive)
                {
                    printf("%-11s|[]", spaces[p_row][p_column]->name);
                    continue;
                }
                else {
                    printf("%-11s|[]", "");
                }
            }
        }
        putchar('\n');
    }
   // printf_s("|[]\n");
}


