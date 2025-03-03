#ifndef AARON_PASSANT
#define AARON_PASSANT
#include "Pawn.h"
#include <stdio.h>
struct PassantPawn {
	
	int get_row() {
		return passant_row;
	}

	int get_column() {
		return passant_column;
	}

	Pawn* get_piece() {
		return pawnthatjustmoved2;
	}
	
	PassantPawn() {
		pawnthatjustmoved2 = NULL;
		passant_row = -1;
		passant_column = -1;
	}
	PassantPawn(Pawn* b_pawnthatjustmoved2, int b_passant_row, int b_passant_column) {
		pawnthatjustmoved2 = b_pawnthatjustmoved2;
		passant_row = b_passant_row;
		passant_column = b_passant_column;
	}
private:
	int passant_row;
	int passant_column;
	Pawn* pawnthatjustmoved2;
};
#endif // !AARON_PASSANT
