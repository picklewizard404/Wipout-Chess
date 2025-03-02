#ifndef AARON_INVALID_PIECE
#define AARON_INVALID_PIECE
#include <stdexcept>
class InvalidPiece : public std::invalid_argument {
private:
	std::string invalid_string;
public:
	InvalidPiece(std::string& name_given): std::invalid_argument(name_given) {
		invalid_string = name_given + +" is not a valid piece to upgrade a pawn to.\n";
	}
	InvalidPiece(Piece* null_piece) : std::invalid_argument("Nonexisting piece") {
		invalid_string = "Tried to move a nonexitend piece.\n";
	}
	const char* what() {
		return invalid_string.c_str();
	}
};
#endif // !AARON_INVALID_PIECE
