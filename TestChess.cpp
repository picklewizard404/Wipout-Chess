/*
* Credit to Catch2 for making it easy to run tests!
*/
#pragma warning(disable:4996)
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
#include "Chess_code\Bishop.h"
#include "Chess_code\Queen.h"
#include "Chess_code\Safety.h"
#include "Chess_code\Teamname.h"
#include "Chess_code\diagnoal_direction.h"
#include "Chess_code\InvalidPiece.h"
#include <iostream>
#include <tuple>
TEST_CASE("Throws errors upgrading pawns to themselves", "[errors]") {
    Board mainboard;
    Team whiteteam = Team(WHITE, &mainboard);
    mainboard.place(&(whiteteam.pawns[0]), 8, 1);
    mainboard.print_board();
    try {
        upgrade_pawn_if_needed(&(whiteteam.pawns[0]), &whiteteam, &mainboard, KING);
    }
    
    catch (InvalidPiece e) {
        printf("%s", e.what());
    }
    catch (std::exception didnt) {
        FAIL("An error wasn't caught?");
    }
    try {
        upgrade_pawn_if_needed(&(whiteteam.pawns[0]), &whiteteam, &mainboard, PAWN);
    }
    catch (InvalidPiece e) {
        printf("%s", e.what());
    }
}

TEST_CASE("The Piece movement is properly changed through inheritance", "[pieces][virtual]") {
    King testKing = King(WHITE);
    Piece* testkpiece = &testKing;
    Board mainboard = Board();
    mainboard.place(testkpiece, testKing.row, testKing.column);
    REQUIRE(testkpiece->can_classmove(1, 5, &mainboard));
}

TEST_CASE("Pawns can catch pawns that jumped over", "[passant]") {
    Board mainboard;
    Pawn wpawn1 = Pawn(WHITE, 5, 1, 1);
    Pawn bpawn2 = Pawn(BLACK, 7, 2, 2);
    mainboard.place(&wpawn1, 5, 1);
    mainboard.place(&bpawn2, 7, 2);
    Move passantmove1 = Move(7, 2, 5, 2, &bpawn2, NULL, false);
    Move passantmove2 = Move(5, 1, 6, 2, &wpawn1, NULL, false);
    printf("Board before black pawn moved 2:\n");
    mainboard.print_board();
    REQUIRE(mainboard.human_move_piece(&passantmove1));
    printf("Board after black pawn moved 2:\n");
    mainboard.print_board();
    REQUIRE(mainboard.pawnthatjustmoved2 == &bpawn2);
    REQUIRE(mainboard.human_move_piece(&passantmove2));
    printf("En Passant!\n");
    mainboard.print_board();
    REQUIRE_FALSE(bpawn2.alive);
}

TEST_CASE("Simple White Winning", "[win]") {
    //TODO
}

TEST_CASE("Queens moving diagonally", "[queen]") {
    Board mainboard;
    Queen testqueen = Queen(WHITE, 5, 8, 1);
}

TEST_CASE("Upgrade a pawn. Pretend you typed.", "[.interactive]") {
    printf("You are the white team and you just landed a pawn on the top right square. Name a piece type to upgrade your pawn to.\n");
    Board mainboard;
    Team whiteteam = Team(WHITE, &mainboard);
    mainboard.place(&(whiteteam.pawns[7]), 8, 8);
    mainboard.print_board();
    TYPE pawn_upgrade_to = upgrade_pawn_if_needed(&(whiteteam.pawns[7]), &whiteteam, &mainboard);
    Piece* whitepawnfromearlier = &(whiteteam.pawns[7]);
    REQUIRE(whitepawnfromearlier != NULL);
    // We no longer point to a pawn, so this pointer is invalid.
    whitepawnfromearlier = NULL;
    Rook* upgradedrook = NULL;
    Knight* upgradedknight = NULL;
    Bishop* upgradedbishop = NULL;
    //TODO ADD QUEENS ONCE YOU MAKE THEM
    switch (pawn_upgrade_to)
    {
    case ROOK:
        upgradedrook = dynamic_cast<Rook*>(whiteteam.pieces[15]);
        REQUIRE(upgradedrook != NULL);
        break;
    case KNIGHT:
        upgradedknight = dynamic_cast<Knight*>(whiteteam.pieces[15]);
        REQUIRE(upgradedknight != NULL);
        break;
    case BISHOP:
        upgradedbishop = dynamic_cast<Bishop*>((whiteteam.pieces[15]));
        REQUIRE(upgradedbishop != NULL);
        break;
    case QUEEN:
        break;
    default:
        break;
    }
    mainboard.print_board();
}

TEST_CASE("Teams are correct", "[teams]") {
    REQUIRE(team_name(BLACK) == "Black");
    REQUIRE(team_name(WHITE) == "White");
}

TEST_CASE("Pieces can be cloned", "[pieces]") {
    Knight k1 = Knight(WHITE, 1, 2, 1);
    Knight k2 = Knight(k1);
    printf("Pieces can be cloned.\n");
    REQUIRE(&k1 != &k2);
    k2.column = 3;
    REQUIRE(k1.column != k2.column);
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
TEST_CASE("Rooks can't move diagonally", "[pieces][rooks]") {
    printf("Rooks are tested.\n");
    Rook testwhite = Rook(WHITE, 1, 1, 1);
    Board mainboard;
    mainboard.place(&testwhite, 1, 1);
    mainboard.spaces[1][1] = NULL;
    REQUIRE(testwhite.can_classmove(2, 2, &mainboard) == false);
}
TEST_CASE("Move Down Bishop", "[bishop][pieces]") {
    Board mainboard;
    Bishop bbish = Bishop(BLACK, 8, 6, 2);
    printf("Pretend a bishop starting at row 8 column 6 is moving down and to the right.\n");
    int row = bbish.row;
    int column = bbish.column;
    move_position_info_diag(&row, &column, DOWN_RIGHT, 1);
    printf("Row: %d, Column: %d.\n", row, column);
    REQUIRE(row == 7);
    REQUIRE(column == 7);
    printf("Now pretend it's moving 3 to the left.\n");
    move_position_info_diag(&row, &column, DOWN_LEFT, 3);
    REQUIRE(row == 4);
    REQUIRE(column == 4);
    printf("Row: %d, Column: %d.\n", row, column);
}
TEST_CASE("Pieces moving diagonally", "[diag][bishop]") {
    Board mainboard;
    Bishop wbish = Bishop(WHITE, 5, 5, 2);
    mainboard.place(&wbish, 5, 5);
    //Assume there is nothing else.
    REQUIRE(can_move_diagnolly(&wbish, DOWN_LEFT, 3, &mainboard));
}

TEST_CASE("Pawns killing enemies", "[diag][pawn]") {
    Board mainboard;
    Bishop wbish = Bishop(WHITE, 5, 5, 1);
    Pawn bpawn = Pawn(BLACK, 6, 6, 1);
    Rook brook = Rook(BLACK, 6, 5, 1);
    mainboard.place(&brook, 5, 6);
    mainboard.place(&wbish, 5, 5);
    mainboard.place(&bpawn, 6, 6);
    mainboard.print_board();
    printf("Can a black pawn be blocked by a black rook?\n");
    REQUIRE_FALSE(bpawn.can_classmove(5, 6, &mainboard));
    printf("Yes!\nCan it move like a bishop?\n");
    REQUIRE_FALSE(bpawn.can_classmove(4, 4, &mainboard));
    printf("No. And it shouldn't.\n");
    printf("Can it move to kill an enemy?\n");
    REQUIRE(bpawn.can_classmove(5, 5, &mainboard));
    printf("Yes.\n");
}

TEST_CASE("White pawn can be blocked by black pawn", "[pawn]") {
    Pawn wpawn = Pawn(WHITE, 2, 1, 1);
    Pawn bpawn = Pawn(BLACK, 3, 1, 1);
    Board mainboard;
    mainboard.place(&wpawn, 2, 1);
    mainboard.place(&bpawn, 3, 1);
    mainboard.print_board();
    printf("Is the white pawn blocked?\n");
    REQUIRE_FALSE(wpawn.can_classmove(3, 1, &mainboard));
    printf("Yes!\n");
}

TEST_CASE("White pawn moving up, black pawns moving down", "[pawn]") {
    Pawn wpawn = Pawn(WHITE, 2, 2, 2);
    Pawn bpawn = Pawn(BLACK, 7, 2, 2);
    Board mainboard;
    mainboard.place(&wpawn, 2, 2);
    mainboard.place(&bpawn, 7, 2);
    int bcolumn = 1;
    while (bcolumn != 4) {
        //printf("Pawn column %d\n", bcolumn);
        if (bcolumn != 2) {
            REQUIRE_FALSE(wpawn.can_classmove(3, bcolumn, &mainboard));
            REQUIRE_FALSE(bpawn.can_classmove(6, bcolumn, &mainboard));
        }
        else
        {
            REQUIRE(wpawn.can_classmove(3, bcolumn, &mainboard));
            REQUIRE(bpawn.can_classmove(6, bcolumn, &mainboard));
        }
        bcolumn++;
    }
    printf("Pawns are moving the way they should.\n");
}

TEST_CASE("Pieces getting blocked by their own team", "[diag][bishop]") {
    Board mainboard;
    Rook wRook = Rook(WHITE, 1, 1, 1);
    Bishop wbish = Bishop(WHITE, 5, 5, 2);
    mainboard.place(&wbish, 5, 5);
    mainboard.place(&wRook, 1, 1);
    REQUIRE_FALSE(can_move_diagnolly(&wbish, DOWN_LEFT, 4, &mainboard));
}

TEST_CASE("Pieces have to stop after killing an enemy moving diagonally", "[diag][pieces]") {
    Board mainboard;
    //Pretend this piece is a bishop.
    Bishop wbish = Bishop();
    Rook enemynear = Rook(BLACK, 4, 4, 1);
    mainboard.place(&wbish, 5, 5);
    mainboard.place(&enemynear, 4, 4);
    printf("A bishop can move diagonally and catch an enemy, ");
    REQUIRE(can_move_diagnolly(&wbish, DOWN_LEFT, 1, &mainboard));
    printf("but after that, he has to stop.\n");
    REQUIRE_FALSE(can_move_diagnolly(&wbish, DOWN_LEFT, 2, &mainboard));
}

TEST_CASE("Pawns can kill only diagonally 1 space", "[diag][pawn]") {
    Board mainboard;
    Pawn bpawn = Pawn(BLACK, 7, 2, 2);
    mainboard.place(&bpawn, 7, 2);
    Pawn wpawn1 = Pawn(WHITE, 6, 1, 1);
    mainboard.place(&wpawn1, 6, 1);
    Pawn wpawn2 = Pawn(WHITE, 6, 2, 2);
    mainboard.place(&wpawn2, 6, 2);
    Pawn wpawn3 = Pawn(WHITE, 6, 3, 3);
    mainboard.place(&wpawn3, 6, 3);
    mainboard.print_board();
    REQUIRE_FALSE(bpawn.can_classmove(5, 2, &mainboard));
    REQUIRE_FALSE(bpawn.can_classmove(6, 2, &mainboard));
    printf("Blocked, can't move down.\n");
    REQUIRE(bpawn.can_classmove(6, 1, &mainboard));
    printf("Can capture diagonally left.\n");
    REQUIRE(bpawn.can_classmove(6, 3, &mainboard));
    printf("Can capture diagonally right.\n");
}
TEST_CASE("Pawns can normally move 2 on the start of their trurn, but not if they're blocked", "[pawn][pawn2]") {
    Board mainboard;
    Pawn wpawn4 = Pawn(WHITE, 2, 4, 4);
    mainboard.place(&wpawn4, 2, 4);
    Move testmove2up = Move(2, 4, 4, 4, &wpawn4, NULL, false);
    Pawn bpawn4 = Pawn(BLACK, 7, 4, 4);
    mainboard.place(&bpawn4, 7, 4);
    Move testmove2down = Move(7, 4, 5, 4, &bpawn4, NULL, false);
    mainboard.print_board();
    REQUIRE(mainboard.human_move_piece(&testmove2up));
    REQUIRE(mainboard.human_move_piece(&testmove2down));
    testmove2up.print_move();
    testmove2down.print_move();
    mainboard.print_board();
    printf("You can normally move a pawn 2 on your first turn.\n");
    mainboard.place(&wpawn4, 2, 4);
    mainboard.place(&bpawn4, 3, 4);
    mainboard.print_board();
    REQUIRE_FALSE(mainboard.human_move_piece(&testmove2up));
    printf("You tried and falied to move up 2.\n");
    mainboard.print_board();
    printf("You can be blocked if an opponent stands directly in front of you.\n");
    mainboard.spaces[2][3] = NULL;
    Bishop bbishop = Bishop(BLACK, 4, 4, 1);
    mainboard.place(&bbishop, 4, 4);
    printf("But what if there was an enemy 2 spaces ahead of you instead?\n");
    mainboard.print_board();
    REQUIRE_FALSE(mainboard.human_move_piece(&testmove2up));
    printf("That doesn't work either.\n");
}
//I can't guarantee an exception is thrown with code, so I have to test it live.
/*
TEST_CASE("I can tell when a move doesn't say what it's moving.", "[moves]") {
    Board mainboard;
    King bKing = King(BLACK);
    REQUIRE_THROWS_MATCHES(Move(1, 5, 8, 5, NULL, &bKing), InvalidMove, Message("InvalidMove::what"));
}*/