// Chess.cpp : This file contains the 'main' function. Program execution begins and ends there.
// I grant credit for Dad. He helped a lot.
#pragma warning(disable:4996)
#include "Chess_non_main.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <ctime>
#include "Safety.h"
#include "Undo_move.h"


using namespace std;

void sleep5();

bool make_kings_hug(Team *current_team, Team*whiteteam, Team*blackteam) {
    printf("Player %d hugs player %d. Both win!\n",
        current_team->color == whiteteam->color ? 1 : 2,
        current_team->enemy_team->color == blackteam->color ? 2 : 1);
    return true;
}

int chess()
{
    Board mainboard;
    Team whiteteam = Team(WHITE, &mainboard);
    Team blackteam = Team(BLACK, &mainboard);
    whiteteam.enemy_team = &blackteam;
    blackteam.enemy_team = &whiteteam;
    Team* public_white_team = &whiteteam;
    Team* public_black_team = &blackteam;
    Piece* piecetomove = NULL;
    
    int m_row = 0;
    int m_column = 0;
    bool okmove = false;
    bool piecefound = false;
    bool isgameover = false;
    char nameofpiecetomove[10];
    int piece = 1;
    bool did_try_tie = false;
    int turn = 1;
    //char current_team = 'w';
    Team* current_team = &whiteteam;

    TYPE type_of_piecetomove = PAWN;
    Game_Status current_status = NEUTRAL;
    
    //Setup
    Piece* wKnight2 = whiteteam.pieces[6];
    Piece* bKnight2 = blackteam.pieces[6];
    Piece* wKing = whiteteam.pieces[4];
    Piece* bKing = blackteam.pieces[4];
    Piece* current_king = wKing;
    bool am_i_in_check = false;
    printf("You can be killed.\n");

    //Begin live test
    /*
    try {
        Move breaks = Move(1, 5, 8, 5, NULL, bKing);
    }
    catch (InvalidMove problem) {
        printf(problem.what());
    }
    // */
    // If playing
    //*
    while (wKing->alive && bKing->alive)
    {
        //say_pieces_of_team(&mainboard, current_team);
        mainboard.print_board();
        printf("%s turn.\n", team_name(current_team->color));
        if (current_team->color == WHITE) {
            //TODO: This is were we check for checkmate:
            bool check_for_checkmate = true;
            if (whiteteam.current_status == CHECK) {
                am_i_in_check = true;
                printf("You are in check!\n");
            }
            if (whiteteam.current_status == CHECKMATE) {
                printf("Black team wins. Good game.\n");
                wKing->alive = false;
                break;
            }
        }
        else if (current_team->color == BLACK) {
            bool check_for_checkmate = true;
            if (blackteam.current_status == CHECK) {
                am_i_in_check = true;
                printf("You are in check!\n");
            }
            if (blackteam.current_status == CHECKMATE) {
                printf("White team wins. Good game.\n");
                bKing->alive = false;
                break;
            }
        }
        printf("Which piece no you want to move? ");
        scanf("%9s", nameofpiecetomove);
        nameofpiecetomove[9] = '\0';
        //Make the name all lowercase.
        nameofpiecetomove[0] = tolower(nameofpiecetomove[0]);
        nameofpiecetomove[1] = toupper(nameofpiecetomove[1]);
        for (int i = 2; i < 9; i++) {
            nameofpiecetomove[i] = tolower(nameofpiecetomove[i]);
        }
        clearinput();
        if (strcmp(nameofpiecetomove, "sUrrender") == 0) {
            printf("You give up. %s team wins!", current_team->enemy_team->full_name);
            sleep5();
            return 0;
        }
        if (strcmp(nameofpiecetomove, "tIe") == 0) {
            printf("Opponent: Do you agree that this match should be called at tie? ");
            scanf("%3s", nameofpiecetomove);
            clearinput();
            nameofpiecetomove[0] = toupper(nameofpiecetomove[0]);
            for (int i = 1; i < 3; i++) {
                nameofpiecetomove[i] = tolower(nameofpiecetomove[i]);
            }
            if (strcmp(nameofpiecetomove, "Yes") == 0) {
                printf("You both give up. Neither team wins!");
                sleep5();
                return 0;
            }
            else {
                did_try_tie = true;
            }
        }
        if (strcmp(nameofpiecetomove, "hUg") == 0) {
            make_kings_hug(current_team, &whiteteam, &blackteam);
            return 0;
        }
        //Find piece with that name
        piecefound = false;
        for (int i = 0; i < 8 && !did_try_tie; i++) {
            if (piecefound) {
                break;
            }
            for (int j = 0; j < 8; j++) {
                if (mainboard.spaces[i][j] != NULL) {
                    //We need to check the type here because we don't yet know what type of piece we have selected.
                    if (strcmp(nameofpiecetomove, mainboard.spaces[i][j]->name) == 0) {
                        //Pretty sure we know the piece is alive because it's on the board.
                        piecetomove = mainboard.spaces[i][j];
                        piecefound = true;
                        type_of_piecetomove = piecetomove->piecetype;
                        break;
                    }
                }
            }
        }
        
        if (piecefound && wrong_team(piecetomove, current_team->color)) {
            printf("Wrong team, dummy!\n");
        } else if (!piecefound) {
            printf("Invalid piece.\n");
        }
        //We found the piece. Now move it.
        if (!wrong_team(piecetomove, current_team->color) && piecefound && !did_try_tie) {
            printf("Where do you want to move %s?\n", nameofpiecetomove);
            printf("Enter your move.\n");

            printf("Row: ");
            scanf("%d", &m_row);
            printf("Column: ");
            scanf("%d", &m_column);
            

            
            switch (type_of_piecetomove)
            {
            case PAWN:
                //If pawn is about to go the end: Set a variable notifying you to upgrade it.
                okmove = ((Pawn*)piecetomove)->can_classmove(m_row, m_column, &mainboard);
                break;
            case ROOK:
                okmove = ((Rook*)piecetomove)->can_classmove(m_row, m_column, &mainboard);
                break;
            case KNIGHT:
                okmove = ((Knight*)piecetomove)->can_classmove(m_row, m_column, &mainboard);
                break;
            case BISHOP:
                okmove = ((Bishop*)piecetomove)->can_classmove(m_row, m_column, &mainboard);
                break;
            case QUEEN:
                break;
            case KING:
                okmove = ((King*)piecetomove)->can_kingmove(m_row, m_column, &mainboard);
                break;
            default:
                saybadmove();
            }

            
            //Where we actually make the move
            Move tried_move;
            tried_move.end_row = m_row;
            tried_move.end_column = m_column;
            tried_move.piece_that_moved = piecetomove;
            tried_move.start_row = piecetomove->row;
            tried_move.start_column = piecetomove->column;
            
            if (okmove && mainboard.is_on_board(m_row, m_column)) {
                mainboard.human_move_piece(&tried_move);
                if (piecetomove->piecetype == PAWN) {
                    upgrade_pawn((Pawn*)piecetomove, current_team);
                }
                //NOTE: THE TEAMS SWAP ON THIS LINE
                current_team = current_team->enemy_team;
                //Was that move safe? IF
                //*
                Game_Status am_I_still_in_check = mainboard.is_in_check(current_team->enemy_team, current_team, &mainboard, false);
                if (am_I_still_in_check != NEUTRAL) {
                    printf("Still in check, silly!\n");
                        //TODO IF UNDO You might have to downgrade a pawn
                        /*
                        undo_move(&tried_move, &mainboard, current_team, &am_I_still_in_check);
                        // We don't switch teams after undoing the move.
                        Switch the teams again
                        // END */
                }
                current_team->enemy_team->current_status = mainboard.is_in_check(current_team->enemy_team, current_team, &mainboard, false);
                current_team->current_status = mainboard.is_in_check(current_team, current_team->enemy_team, &mainboard);
                // END
                // */
            } else {
                saybadmove();
            }

            
        }
        else {
            if ((piecetomove != NULL)) {
                if (!piecetomove->alive) {
                    printf("That piece is dead! Can't move it anymore;\n");
                }
            }
            else {
                if (did_try_tie) {
                    printf("Your opponent doen't want to quit yet.\n");
                }
                else {
                    printf("Wrong team, silly.\n");
                }
            }
        }
        did_try_tie = false;
    }
    if (whiteteam.the_king.row == blackteam.the_king.row
        && whiteteam.the_king.column == blackteam.the_king.column) {
        printf("%s king hugged the %s king. Happy ending!", current_team->enemy_team->full_name, current_team->full_name);
        return 0;
    }
    if (!whiteteam.the_king.alive) {
        printf("Black team wins.\n");
        return 2;
    }
    if (!blackteam.the_king.alive) {
        printf("White team wins.\n");
        return 1;
    }
    return 0;
}

void sleep5() {
    Sleep(5 * CLOCKS_PER_SEC);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file