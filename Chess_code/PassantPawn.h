#ifndef AARON_PASSANT
#define AARON_PASSANT
#include "Pawn.h"
#include <stdio.h>
struct PassantPawn {
	Pawn* pawnthatjustmoved2;
	int passant_row;
	int passant_column;
	void reset() {
		pawnthatjustmoved2 = NULL;
		passant_row = -1;
		passant_column = -1;
	}
	PassantPawn() {
		reset();
	}
	PassantPawn(Pawn* b_pawnthatjustmoved2, int b_passant_row, int b_passant_column) {
		pawnthatjustmoved2 = b_pawnthatjustmoved2;
		passant_row = b_passant_row;
		passant_column = b_passant_column;
	}
};
#endif // !AARON_PASSANT
