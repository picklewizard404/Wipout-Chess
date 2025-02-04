#include "Team.h"
#include <stdio.h>
#pragma warning(disable:4996)
//NOTE: This function is called EVERY time a pawn is moved, and it does nothing is the pawn is not on it.
void upgrade_pawn(Pawn* to_upgrade, Team* team_owner) {
	//TODO Add other playable classes.
	const char* classes[] = { "Rook",  "Bishop", "Knight" };
	/*NOTE: Pawns are in posisitions 2:1-2:8,
	        which means the team treats them as pieces 8-15*/
	int final_pawn_row = 8;
	if (to_upgrade->team == BLACK) {
		final_pawn_row = 1;
	}
	if (to_upgrade->row != final_pawn_row) {
		return;
	}
	//Upgrade the pawn here.
	for (int i = 8; i < 16; i++) {
		if (team_owner->pieces[i] == to_upgrade) {
			TYPE inputtype = ROOK; //TODO Change this to let the user's selected piece type.
			if (inputtype == ROOK) {
				//team_owner->pieces[i] = some new rook
			}
		}
	}
	return;
}

void perform_upgrade(Pawn* to_upgrade, Piece* to_piece, Team* team_owner) {
	for (int i = 8; i < 16; i++) {
		if (team_owner->pieces[i] == to_upgrade) {
			switch (to_upgrade->piecetype)
			{
			case ROOK:
				team_owner->pawns[i-8] = Rook(*(Rook*)to_piece);
				team_owner->pieces[i] = &team_owner->pawns[i - 8];
			default:
				break;
			}
		}
	}
}

void upgade_pawn_to(Pawn* to_upgrade, TYPE new_class, Team* team_owner) {
	Piece* upgraded = NULL;
	switch (new_class)
	{
	case PAWN:
		break;
	case ROOK:
	{
		Rook newRook = Rook(to_upgrade->team, to_upgrade->row, to_upgrade->column, 2 + to_upgrade->count);
		sprintf(newRook.name, "%cRookp%d", to_upgrade->team, to_upgrade->count);
		upgraded = &newRook;
	}
		break;
	case KNIGHT:
		break;
	case BISHOP:
		break;
	case QUEEN:
		break;
	case KING:
		break;
	case EMPTY:
		break;
	default:
		break;
	}
	//Copy an object HERE.
	if (upgraded != NULL) {
		perform_upgrade(to_upgrade, upgraded, team_owner);
	}
}