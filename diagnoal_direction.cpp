#include "diagnoal_direction.h"

void calculate_diag_mv(int* row, int* column, DIAGONAL direction, int count) {
	switch (direction) {
	case UP_LEFT:
		*row = count+ *row;
		*column = (-1 * count) + *column;
		break;
	case UP_RIGHT:
		*row = count + *row;
		*column = count + *column;
		break;
	case DOWN_LEFT:
		*row = (-1 * count) + *row;
		*column = ( - 1 * count) + *column;
		break;
	case DOWN_RIGHT:
		*row = (-1 * count) + *row;
		*column = count + *column;
		break;
	}
}