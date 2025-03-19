#include "Piece.h"
#include "Knight.h"
#include "Board.h"

#include <iostream>
#include "Safety.h"
#include "Check_vs_Checkmate.h"
#include "CastleMove.h"
#include "Pawn_Upgrader.h"
#include "InvalidPiece.h"
#include <string>
using namespace std;

Board::Board() {
    turn_number = 1;
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
//Note that this does NOT count as making a move.
void Board::place(Piece* piece, int row, int column) {
    spaces[piece->row - 1][piece->column - 1] = NULL;
    spaces[row - 1][column - 1] = piece;
    piece->row = row;
    piece->column = column;
}

bool Board::does_have_any_piece(int row, int column) const {
    return spaces[row - 1][column - 1] != NULL;
}

bool Board::no_ally_there(COLOR my_team, int row, int column) const {
    if (spaces[row - 1][column - 1] != NULL) {
        return spaces[row - 1][column - 1]->team != my_team;
    }
    else {
        return true;
    }
}

int Board::get_turn()
{
    return turn_number;
}

void Board::next_turn()
{
    turn_number++;
}

Game_Status Board::is_in_check(Team* my_team, Team* enemy_team, bool check_for_checkmate)  {
    //Todo: Use info HERE to try every possible move of my_team
    int mkcolumn = my_team->the_king.column;
    int mkrow = my_team->the_king.row;
    bool is_hugging_allowed = false;
    for (int i = 0; i < 16; i++) {
        if (enemy_team->pieces[i] != 0) {
            if (enemy_team->pieces[i]->can_classmove(mkrow, mkcolumn, this)) {
                if (false && check_for_checkmate) {
                    return try_to_escape(my_team, enemy_team, this);
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
//NOTE: This is only used in tests?
//It had better not change any state.
Move Board::make_move(Piece* piece_that_moved, int erow, int ecolumn) {
    if (piece_that_moved == NULL) {
        throw InvalidPiece(piece_that_moved);
    }
    //Pawn* passantpawnpiece = NULL;
    int passantrow = -1;
    int passantcolumn = -1;
    bool createnewpassant = false;
    /*if (piece_that_moved->piecetype == PAWN) {
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
    }*/
    
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
	if (move_to_make == NULL) {
		throw InvalidPiece(NULL);
	}
    space piece = move_to_make->piece_that_moved;
    int b_row = move_to_make->end_row;
    int b_column = move_to_make->end_column;
    bool changepast = false;
	CastleMove* castlemove = dynamic_cast<CastleMove*>(move_to_make);
	if (castlemove != NULL) {
        switch (castlemove->which_side_you_castled)
        {
		case LEFT:
            place(castlemove->rook_that_moved, castlemove->rook_that_moved->row, 4);
			break;
		case RIGHT:
			place(castlemove->rook_that_moved, castlemove->rook_that_moved->row, 6);
        default:
            break;
        }
        //TODO: MOVE YOUR KING TOO
		return true;
	}
    if (piece != NULL) {
        int p_column = piece->column;
        int p_row = piece->row;
        if (castlemove != NULL) {
            if (castlemove->piece_that_moved->piecetype == NULL) {
				throw InvalidPiece(castlemove->piece_that_moved);
            }
			if (castlemove->piece_that_moved->piecetype != KING) {
				throw InvalidMove(string(castlemove->piece_that_moved->name) + string("is NOT your king!"));
			}
            if (castlemove->piece_that_moved->first_turn_i_moved() != -1) {
                throw InvalidMove(string("Too late to castle. You've already moved!"));
            }
            return true;
        }
        bool can_move = piece->can_classmove(b_row, b_column, this);
		if (move_to_make->piece_that_moved == NULL) {
			throw InvalidPiece(move_to_make->piece_that_moved);
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
                //Remember the piece I landed on in case this move has to be undone.
                move_to_make->piece_landed_on = spaces[b_row - 1][b_column - 1];
            }

            //Check if an en passant was made.
            if (passantpawn.get_piece() != NULL) {
                if (b_row == passantpawn.get_row() && b_column == passantpawn.get_column() && passantpawn.get_piece()->team != piece->team) {
                    kill_passant();
                }
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
                if (passantpawn.get_turn_made() < turn_number) {
                    prevepassant = passantpawn;
                    passantpawn = PassantPawn();
                }
            }
            //Either way move the piece.
			piece->know_i_moved(turn_number);
            spaces[move_to_make->start_row - 1][move_to_make->start_column - 1] = NULL;
            place(piece, b_row, b_column);
            piece->know_i_change_position(b_row, b_column, turn_number);
            if (passantpawn.get_piece() != NULL) {
                //Apply passant if needed.
                if (piece->piecetype == PAWN && b_row == passantpawn.get_row() && b_column == passantpawn.get_column() && piece->team != passantpawn.get_piece()->team) {
                    kill_passant();
                }
                else if (passantpawn.get_piece()->team != piece->team) {
                    prevepassant = passantpawn;
                    //Create a new instance of a readonly class to reset it.
                    passantpawn = PassantPawn();
                }
            }


            
            //Maybe the passant didn't happen.
            if (turn_number > passantpawn.get_turn_made() && passantpawn.get_piece() != NULL) {
                prevepassant = passantpawn;
                passantpawn = PassantPawn();
            }
            
            //Maybe we set one up for our opponent though.
            if (piece->piecetype == PAWN) {
                if (piece->team == WHITE && move_to_make->start_row == 2 && move_to_make->end_row == 4) {
                    passantpawn = PassantPawn((Pawn*)piece, 3, piece->column, turn_number);
                }
                else if (piece->team == BLACK && move_to_make->start_row == 7 && move_to_make->end_row == 5) {
                    passantpawn = PassantPawn((Pawn*)piece, 6, piece->column, turn_number);
                }
            }

            //Now we know we're done moving.
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

int Board::current_turn() const
{
    return turn_number;
}

//TODO IF UNDO You might have to downgrade a pawn.
// Check if the piece that moved was a pawn and if it moved to the end of the board.
// If i did then delete team_owner->upgraded_pieces[move_i_made->piece_that_moved->count-8]
// THEN SET IT TO NULL RIGHT AFTER!
//TODO UNDO CASTLING BY DOING THE OPPOSITE OF CASTLING PLACING PIECES BACK WHERE THEY WERE
void Board::undo_move(Move* move_i_made) {
    passantpawn = prevepassant;
    prevepassant = PassantPawn();
    int previousrow = move_i_made->end_row;
    int previouscolumn = move_i_made->end_column;
    Piece* piecethatmoved = move_i_made->piece_that_moved;
    
    if (move_i_made->piece_landed_on != NULL) {
        move_i_made->piece_landed_on->alive = true;
    }
    place(piecethatmoved, move_i_made->start_row, move_i_made->start_column);
    piecethatmoved->row = move_i_made->start_row;
    piecethatmoved->column = move_i_made->start_column;
    turn_number--;
}
static void print_piece(Piece *piece /*bool islast*/) {
    char piecename[11] = "          ";
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
void Board::print_board() const {
    const int length_of_name = 12;
    const int number_of_spaces = 8;
    bool firstcolumn = true;

    //Top row.
    for (int i = 0; i < 6 + length_of_name * (number_of_spaces - 1); i++) {
        printf("-");
    }
    printf("\n");
    for (int row = 7; row >= 0; row--) {
        firstcolumn = true;
        if (row == 7 || row == 0) {
            printf("[]");
        }
        else if (firstcolumn) {
            printf("| ");
        }
        else {
            printf("  ");
        }
        for (int column = 0; column <= 7; column++) {
            if (firstcolumn && column != 0) {
                firstcolumn = false;
            }
            print_piece(spaces[row][column]);
        }
        printf("\n");
        if (row > 0) {
            printf("|");
            for (int i = 0, minus = 0; i < 4 + length_of_name * (number_of_spaces-1); i++) {
                if ((i % length_of_name) != length_of_name-1 - minus) {
                    printf("_");
                }
                else {
                    printf("|");
                    minus++;
                }
            }
            printf("|\n");
        }
    }
    //Bottom row
    for (int i = 0; i < 6 + length_of_name * (number_of_spaces - 1); i++) {
        printf("-");
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
// FINAL STEP FINISHING THE GAME
// MAKE SURE TO BACK UP THE STARTING passantpawn AND prevepassant...
//      AND SET THEM BACK AFTER UNDOING EVERY MOVE
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
                if (is_in_check(my_team, enemy_team, false) == NEUTRAL) {
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