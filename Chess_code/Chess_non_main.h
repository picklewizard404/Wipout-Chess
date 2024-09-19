#include "Piece.h"
#include "Board.h"
#include "Team.h"
#include "move_diagnolly.h"
#include "diagnoal_direction.h"
void tellmeaspace(Piece* piecetomove);
void saybadmove();
void say(int column, int row, Board mainboard);
COLOR char_opposing_team(COLOR team);
void say_pieces_of_team(Board* mainboard, COLOR current_team);
bool wrong_team(space piecetomove, COLOR current_team);
COLOR enemy_team(COLOR my_team);
bool do_move(Team** current_team, Board* mainboard, Piece* piecetomove, int m_row, int m_column, Team* public_white_team, Team* public_black_team, bool switch_teams = true);
bool check_piece(Board* mainboard, Piece** piecetomove, int row, int column, char nameofpiecetomove[]);
const char* team_name(COLOR team);
