#include "Piece.h"
#include <exception>
#include <string>
#ifndef DEFINE_MOVE
#define DEFINE_MOVE
struct Move
{
	int start_row;
	int start_column;
	int end_row;
	int end_column;
	Piece* piece_that_moved;
	Piece* piece_landed_on;
	Move();
	Move(int srow, int scolumn, int erow, int ecolumn,
		Piece* mpiece_that_moved, Piece* mpiece_landed_on,
		bool sayimoved = true);
	void print_move();
};
#endif // !DEFINE_MOVE
#ifndef INVALID_MOVE
#define INVALID_MOVE
class InvalidMove: std::exception {
	int row;
	int column;
	std::string problem;
public:
	const char* what() const noexcept {
		return problem.c_str();
	};
	InvalidMove(int b_row, int b_column);
};
#endif // !INVALID_MOVE

