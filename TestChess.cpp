/*
* Credit to Catch2 for making it easy to run tests!
*/

#include "Catch2_code\catch_amalgamated.hpp"
#include "Chess_code\Chess.h"
#include "Chess_code\Board.h"
#include "Chess_code\move_diagnolly.h"
#include "Chess_code\Chess_non_main.h"
#include "Chess_code\Piece.h"
#include "Chess_code\King.h"
#include "Chess_code\Move_horizontally.h"
#include "Chess_code\Team.h"
#include "Chess_code\Pawn.h"
#include "Chess_code\Knight.h"
#include "Chess_code\Straight_Line.h"
#include "Chess_code\Rook.h"
#include "Chess_code\Safety.h"
#include "Chess_code\Teamname.h"
#include "Chess_code\diagnoal_direction.h"
TEST_CASE("Teams are correct", "[teams]") {
    REQUIRE(team_name(BLACK) == "Black");
    REQUIRE(team_name(WHITE) == "White");
}
TEST_CASE("Kings can hug", "[pieces][kings]") {
    printf("Kings are tested.\n");
    Board mainboard = Board();
    Team blackteam = Team(BLACK, &mainboard);
    Team whiteteam = Team(WHITE, &mainboard);
    whiteteam.enemy_team = &blackteam;
    blackteam.enemy_team = &whiteteam;
    REQUIRE(make_kings_hug(&whiteteam, &whiteteam, &blackteam) == true);
}
TEST_CASE("Rooks can't move diagnolly", "[pieces][rooks]") {
    printf("Rooks are tested.\n");
    Rook testwhite = Rook(WHITE, 1, 1, 1);
    Board mainboard;
    mainboard.place(&testwhite, 1, 1);
    mainboard.spaces[1][1] = NULL;
    REQUIRE(testwhite.can_classmove(2, 2, &mainboard) == false);
}
TEST_CASE("Move Down right", "[math][bishops]") {
    Board mainboard;
    King bking = King(BLACK);
    int row = bking.row;
    int column = bking.column;
    calculate_diag_mv(&row, &column, DOWN_LEFT, 1);
    printf("Row: %d, Column: %d,\n", row, column);
    REQUIRE(row == 7);
    REQUIRE(column == 4);
    calculate_diag_mv(&row, &column, DOWN_RIGHT, 1);
    REQUIRE(row == 6);
    REQUIRE(column == 5);
    printf("Row: %d, Column: %d,\n", row, column);
}