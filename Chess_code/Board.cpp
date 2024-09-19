#include "Piece.h"
#include "Knight.h"
#include "Board.h"

#include <iostream>
#include "Safety.h"
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

bool Board::is_in_check(King *king_of_team_1, Team *team_2, Board *mainboard) {
    int column = king_of_team_1->column;
    int row = king_of_team_1->row;
    for (int i = 0; i < 16; i++) {
        if (team_2->pieces[i] != 0) {
            switch (team_2->pieces[i]->piecetype) {
            case KING:
                if ((King*)team_2->pieces[i]->can_classmove(row, column, mainboard)) {
                    return true;
                }
            case KNIGHT:
                if ((Knight*)team_2->pieces[i]->can_classmove(row, column, this)) {
                    return true;
                }
            case ROOK:
                if ((Rook*)team_2->pieces[i]->can_classmove(row, column, this)) {
                    return true;
                }
            //Empty spaces go here and do not threaten you
            default:
                continue;
            }
        }
    }
    return false;
}
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
bool Board::move(space piece, int b_row, int b_column, bool* landed_on_king) {
    if (piece != NULL) {
        int p_column = piece->column;
        int p_row = piece->row;
        bool can_move = false;
        can_move = can_move || piece->can_classmove(b_row, b_column, this);
        if (piece != NULL) {
            if (piece->piecetype == KING) {
                can_move = can_move || ((King*)piece)->can_kingmove(b_row, b_column, this, landed_on_king);
            }
        }
        if (can_move) {
            // If you landed on a piece on your team:
            if (piece->do_team_match(spaces[b_row - 1][b_column - 1])) {
                printf("You have a piece there, silly!\n");
                return false;
            }

            //Opposing team:
            if (spaces[b_row - 1][b_column - 1] != NULL) {
                spaces[b_row - 1][b_column - 1]->alive = false;
                //Remember the fact that I was there in case this move has to be redone.
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


