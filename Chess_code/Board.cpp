#include "Piece.h"
#include "Knight.h"
#include "Board.h"

#include <iostream>
#include "Safety.h"
#include "Check_vs_Checkmate.h"
#include "Move.h"
#include "Pawn_Upgrader.h"
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
//Teleporting pieces is useful for testing.
void Board::place(Piece* piece, int row, int column) {
    spaces[piece->row - 1][piece->column - 1] = NULL;
    spaces[row - 1][column - 1] = piece;
    piece->row = row;
    piece->column = column;
}

bool Board::does_have_any_piece(int row, int column) {
    return spaces[row - 1][column - 1] != NULL;
}

bool Board::no_ally_there(COLOR my_team, int row, int column) {
    if (spaces[row - 1][column - 1] != NULL) {
        return spaces[row - 1][column - 1]->team != my_team;
    }
    else {
        return true;
    }
}

//TODO Check where this function is called and make sure teams are passed.
//Maybe I am calling this function for the wrong team?
//The team we are checking check for is my_team.
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
                    if (false && check_for_checkmate) {
                        return try_to_escape(my_team, enemy_team, mainboard);
                    }
                    else return CHECK;
                }
            case KNIGHT:
                if ((Knight*)enemy_team->pieces[i]->can_classmove(row, column, this)) {
                    if (false && check_for_checkmate) {
                        return try_to_escape(my_team, enemy_team, mainboard);
                    }
                    else return CHECK;
                }
            case ROOK:
                if ((Rook*)enemy_team->pieces[i]->can_classmove(row, column, this)) {
                    if (false && check_for_checkmate) {
                        return try_to_escape(my_team, enemy_team, mainboard);
                    }
                    else return CHECK;
                }
            case BISHOP:
                if ((Bishop*)enemy_team->pieces[i]->can_classmove(row, column, this)) {
                    if (false && check_for_checkmate) {
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
bool Board::human_move_piece(Move* move_to_make) {
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
            spaces[move_to_make->start_row - 1][move_to_make->start_column - 1] = NULL;
            place(piece, b_row, b_column);
            piece->know_i_change_position(b_row, b_column);
            return true;
        }
        return false;
    }
    return false;
}
void undo_move(Move* move_to_make) {
    Piece* piece = move_to_make->piece_that_moved;
}
void print_piece(Piece *piece /*bool islast*/) {
    char piecename[11];
    piecename[10] = '\0';
    if (piece != NULL) {
        int charofname = 0;
        for (; piece->name[charofname] != '\0' && charofname < 10; charofname++) {
            if (piece->name[charofname] == '\0') {
                break;
            }
            piecename[charofname] = piece->name[charofname];
        }
        while (charofname < 10) {
            piecename[charofname] = ' ';
            charofname++;
        }
        printf("%s|", piecename);
    }
    else printf("          |");
}
void Board::print_board() {
    const int length_of_name = 12;
    const int number_of_spaces = 8;
    for (int row = 7; row >= 0; row--) {
        if (row == 7 || row == 0) {
            printf("[]");
        }
        else {
            printf("  ");
        }
        for (int column = 0; column <= 7; column++) {
            print_piece(spaces[row][column]);
        }
        printf("\n");
        if (row > 0) {
            printf("|");
            for (int i = 0; i < 4 + length_of_name * (number_of_spaces-1); i++) {
                printf("_");
            }
            printf("|\n");
        }
    }
    printf("\n");
}

void undo_board_move(Move* tried_move, Board* mainboard, Team* team_undoing_move, Game_Status* game_status) {
    mainboard->place(tried_move->piece_that_moved, tried_move->start_row, tried_move->start_column);
    tried_move->piece_that_moved->row = tried_move->start_row;
    tried_move->piece_that_moved->column = tried_move->start_column;
    if (tried_move->piece_landed_on != NULL) {
        tried_move->piece_landed_on->alive = true;
        mainboard->place(tried_move->piece_landed_on, tried_move->end_row, tried_move->end_column);
        *game_status = mainboard->is_in_check(team_undoing_move->enemy_team, team_undoing_move, mainboard, false);
    }
}

//TODO I UNDO MOVES WRONGLY
//By the time I undo the move, I have already forgotten the piece I landed on.
Game_Status Board::try_to_escape(Team* my_team, Team* enemy_team, Board* mainboard) {
    Piece* one_of_my_pieces;
    Move tried_move;
    for (int i = 0; i < 16; i++)
    {
        one_of_my_pieces = my_team->pieces[i];
        if (!one_of_my_pieces->alive) continue;
        tried_move.piece_that_moved = my_team->pieces[i];
        tried_move.start_row = my_team->pieces[i]->row;
        tried_move.start_column = my_team->pieces[i]->column;
        for (int tryrow = 1; tryrow <= 8; tryrow++) {
            tried_move.end_row = tryrow;
            for (int trycolumn = 1; trycolumn <= 8; trycolumn++) {
                tried_move.end_column = trycolumn;
                // We know we can go here, so we might as well try.
                human_move_piece(&tried_move);
                if (is_in_check(my_team, enemy_team, this, false) == NEUTRAL) {
                    undo_move(&tried_move);
                    return CHECK;
                }
            }
        }
    }
    //TEMP Here I know the move that saved me and I haven't actually made it on purpose yet,
    // so I need to undo the move before exiting the loop/
    // END OF IMAGINARY LOOP
    return CHECK;
};