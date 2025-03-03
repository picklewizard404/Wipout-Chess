#include "Piece.h"
#include "Knight.h"
#include "Board.h"

#include <iostream>
#include "Safety.h"
#include "Check_vs_Checkmate.h"
#include "Move.h"
#include "Pawn_Upgrader.h"
#include "InvalidPiece.h"
using namespace std;

Board::Board() {
    turn_number = 0;
    passant_number = -1;
    whiteturn = true;
    threatens_white = NULL;
    threatens_black = NULL;
    previous_move = Move();
    passantpawn = PassantPawn();
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
    int mkcolumn = my_team->the_king.column;
    int mkrow = my_team->the_king.row;
    bool is_hugging_allowed = false;
    for (int i = 0; i < 16; i++) {
        if (enemy_team->pieces[i] != 0) {
            if (enemy_team->pieces[i]->can_classmove(mkrow, mkcolumn, this)) {
                if (false && check_for_checkmate) {
                    return try_to_escape(my_team, enemy_team, mainboard);
                }
                else return CHECK;
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
//TODO I AM FAILING HERE. THE PASSANT PAWN IS NOT RESETTING.
Move Board::make_move(Piece* piece_that_moved, int erow, int ecolumn) {
    if (piece_that_moved == NULL) {
        throw InvalidPiece(piece_that_moved);
    }
    Pawn* passantpawnpiece = NULL;
    int passantrow = -1;
    int passantcolumn = -1;
    bool createnewpassant = false;
    if (piece_that_moved->piecetype == PAWN) {
        if (piece_that_moved->team == WHITE && piece_that_moved->row == 2 && erow == 4) {
            passantpawnpiece = (Pawn*)piece_that_moved;
            passantrow = 4;
            passantcolumn = piece_that_moved->column;
            createnewpassant = true;
        }
        else if(piece_that_moved->team == BLACK && piece_that_moved->row == 7 && erow == 5)
        {
            passantpawnpiece = (Pawn*)piece_that_moved;
            passantrow = 5;
            passantcolumn = piece_that_moved->column;
            createnewpassant = true;
        }
        if (createnewpassant) {
            passantpawn = PassantPawn(passantpawnpiece, passantrow, passantcolumn);
        }
        
    }
    
    Move nextmove = Move(piece_that_moved->row, piece_that_moved->column, erow, ecolumn, 
        piece_that_moved, spaces[erow - 1][ecolumn - 1],
        false);
    return nextmove;
}
/*
b_column and b_row range from 1 to 8. We subtract 1 whenever we reference a space on the board.
That's because a person starts counting spaces with 1 but the computer starts counting with 0.
Detects if the move lands on an enemy piece and stores the enemy piece in the move in case it is un-done.

TODO ADD SUPPORT FOR ON PASONT RULE.
If pawn moves 2, save a Piece pointer to the pawn and the spot that it would have
been in if it moved 1. I call that space SS in this example.
If the opponent lands on SS THE EXACT TURN AFTER the pawn moved, the pawn dies.
That means setting the pawn and SS both to NULL after making a move on the opponent's turn.
Check if you did the passant before clearing it and hopefully everything will work.
*/
bool Board::human_move_piece(Move* move_to_make) {
    space piece = move_to_make->piece_that_moved;
    int b_row = move_to_make->end_row;
    int b_column = move_to_make->end_column;
    bool changepast = false;
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

            //Think about passantpieces moving through a pipe.
            //First, you forget your oldest memory.
            //Then your current memory gets older.
            //Then you make a new memory.
            if (prevepassant.get_piece() != NULL) {
                if (prevepassant.get_piece()->team == piece->team) {
                    prevepassant = PassantPawn();
                }
            }
            
            if (passantpawn.get_piece() != NULL) {
                if (passantpawn.get_piece()->team == piece->team) {
                    if (prevepassant.get_piece() == NULL) {
                        changepast = true;
                    }
                    else if (prevepassant.get_piece()->team == piece->team) {
                        changepast = true;
                    }
                    if (changepast) {
                        prevepassant = passantpawn;
                    }
                }
            }
            //Either way move the piece.
            spaces[move_to_make->start_row - 1][move_to_make->start_column - 1] = NULL;
            place(piece, b_row, b_column);
            piece->know_i_change_position(b_row, b_column);
            if (passantpawn.get_piece() != NULL) {
                //Apply passant if needed.
                if (piece->piecetype == PAWN && b_row == passantpawn.get_row() && b_column == passantpawn.get_column() && piece->team != passantpawn.get_piece()->team) {
                    kill_passant();
                }
                else if (passantpawn.get_piece()->team != piece->team) {
                    prevepassant = passantpawn;
                    //Create a new intstance of a readonly class to reset it.
                    passantpawn = PassantPawn();
                    //Maybe the mistake is here?
                }
            }


            //Now we know we're done moving.
            //Maybe the passant didn't happen.
            if (turn_number > passant_number && passantpawn.get_piece() != NULL) {
                prevepassant = passantpawn;
                passantpawn = PassantPawn();
            }
            turn_number++;
            return true;
        }
        return false;
    }
    return false;
}

void Board::kill_passant() {
    passantpawn.get_piece()->alive = false;
    spaces[passantpawn.get_piece()->row - 1][passantpawn.get_piece()->column - 1] = NULL;
    //This is safe because doing a passant will NEVER be followed by a passant.
    passantpawn = PassantPawn();
}

void Board::undo_move(Move* move_i_made) {
    //TODO PRESERVE YOUR EN PASSANT PAWN IN THIS FUNCTION
    passantpawn = prevepassant;
    prevepassant = PassantPawn();
    Piece* piecethatmoved = move_i_made->piece_that_moved;
    piecethatmoved->row = move_i_made->start_row;
    piecethatmoved->column = move_i_made->start_column;
    if (move_i_made->piece_landed_on != NULL) {
        move_i_made->piece_landed_on->alive = true;
    }
    //passantpawn = prevpassantpawn;
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

void Board::print_passant(bool* testprinted) {
    *testprinted = false;
    Pawn* passantpawnpiece = passantpawn.get_piece();
    if (passantpawnpiece != NULL) {
        printf("%s just jumped over row %d, column %d\n", passantpawnpiece->name, passantpawn.get_row(), passantpawn.get_column());
        *testprinted = true;
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