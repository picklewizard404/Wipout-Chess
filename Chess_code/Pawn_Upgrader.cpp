#include "Team.h"
#include <stdio.h>
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