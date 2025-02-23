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
	const char* what() {
		return invalid_string.c_str();
	}
};
#endif // !AARON_INVALID_PIECE
