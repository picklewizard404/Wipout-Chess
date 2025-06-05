// Chess.cpp : This file contains the 'main' function. Program execution begins and ends there.
// I grant credit for Dad. He helped a lot.
#pragma warning(disable:4996)
#include "Chess_non_main.h"
#include <ctype.h>
#include <tuple>
#include <stdio.h>
#include <string.h>

#include <ctime>
#include "Safety.h"
#include "Undo_move.h"
#include <chrono>
#include <thread>
#include "InvalidPiece.h"

using namespace std;

void print_how_to_hug() {
    printf("To hug the king you just landed on instead of defeating him,\njust type Yes.\n");
}

bool make_kings_hug(Team *current_team, Team*whiteteam, Team*blackteam) {
    printf("%s player hugs %s player. Both win!\n",
        current_team->color == whiteteam->color ? whiteteam->team_name() : blackteam->team_name(),
        current_team->enemy_team->color == blackteam->color ? blackteam->team_name() : whiteteam->team_name());
    return true;
}

void sleep5() {
    std::this_thread::sleep_for(std::chrono::seconds(5));
}

int chess(bool should_load_man)
{
    /*NOTE THAT YOU CAN'T CASTLE WHILE IN CHECK.
    * THE BOARD SHOULD KNOW WHATE TEAMS ARE IN CHECK AND PREVENT CASTLING IF THE TEAM TRYING TO CASTLE IS IN CHECK.
    * IT IS POSSIBLE FOR BOTH TEAMS TO BE IN CHECK, SO THE BOARD NEEDS 2 BOOLEANS TO TELL WHETHER OR NOT the current team is in check.
    * */
    Board mainboard;
    Team whiteteam = Team(COLOR::WHITE, &mainboard);
    Team blackteam = Team(COLOR::BLACK, &mainboard);
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
    bool did_try_castle = false;
    int turn = 1;
    //char current_team = 'w';
    Team* current_team = &whiteteam;

    TYPE type_of_piecetomove = PAWN;
    Game_Status current_status = NEUTRAL;

	bool is_loaded = !should_load_man;
	bool do_load = false;
    
    //Setup
    Piece* wKnight2 = whiteteam.pieces[6];
    Piece* bKnight2 = blackteam.pieces[6];
    Piece* wKing = whiteteam.pieces[3];
    Piece* bKing = blackteam.pieces[3];
    Piece* current_king = wKing;
    bool am_i_in_check = false;
    if (should_load_man) {
		/*TODO FINISH LETTING YOU SET UP THE GAME. YOU NEED TO SET is_loaded TO true
		* when you are done loading the game.
        Skip the player's movement of pieces when they load the game instead, 
        just like you skip certain print statements when you castle. */
		printf("Type cteam to say you're done loading the previous game and the current team starts,...\n");
        printf("...or type oteam to say you're done loading the previous game and the other team starts.\n");
    }
    printf("\nGod answered my prayers and helped me make this game. He deserves credit!\n\n");
    
    //*
    while (wKing->alive && bKing->alive)
    {
        //say_pieces_of_team(&mainboard, current_team);
        mainboard.print_board();
        did_try_castle = false;
        printf("%s turn.\n", team_name(current_team->color));
        if (current_team->color == COLOR::WHITE) {
            //We already checked if we are in check or checkmate at the end of our opponent's turn.
            if (whiteteam.current_status == Game_Status::CHECK) {
                am_i_in_check = true;
                printf("You are in check!\n");
            }
            if (whiteteam.current_status == Game_Status::CHECKMATE) {
                printf("Black team wins. Good game.\n");
                wKing->alive = false;
                break;
            }
        }
        else if (current_team->color == COLOR::BLACK) {
            if (blackteam.current_status == Game_Status::CHECK) {
                am_i_in_check = true;
                printf("You are in check!\n");
            }
            if (blackteam.current_status == Game_Status::CHECKMATE) {
                printf("White team wins. Good game.\n");
                bKing->alive = false;
                break;
            }
        }
        printf("Which piece no you want to move? ");
        std::ignore = scanf("%9s", nameofpiecetomove);
        nameofpiecetomove[9] = '\0';
        //Make the name all lowercase.
        nameofpiecetomove[0] = tolower(nameofpiecetomove[0]);
        nameofpiecetomove[1] = toupper(nameofpiecetomove[1]);
        for (int i = 2; i < 10; i++) {
            nameofpiecetomove[i] = tolower(nameofpiecetomove[i]);
        }
        clearinput();

		if (should_load_man) {
			if (strcmp(nameofpiecetomove, "cTeam") == 0) {
                if (is_loaded) {
					printf("You are already done loading.\n");
                }
                //else {
					do_load = true;
                //}
			}
			else if (strcmp(nameofpiecetomove, "oTeam") == 0) {
                if (is_loaded) {
                    printf("You are already done loading.\n");
                }
                else {
					current_team = current_team->enemy_team;
                    
                }
                do_load = true;
			}
		}
        if (strcmp(nameofpiecetomove, "sUrrender") == 0) {
            printf("You give up. %s team wins!", current_team->enemy_team->full_name);
            sleep5();
            return 0;
        }
        if (strcmp(nameofpiecetomove, "tIe") == 0) {
            printf("Opponent: Do you agree that this match should be called at tie? ");
            std::ignore = scanf("%3s", nameofpiecetomove);
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

        //Maybe you are trying to castle.
        CastleMove castle_move;
        if (strcmp(nameofpiecetomove, "cAstle") == 0) {
            bool have_decided_direction = false;
            did_try_castle = true;
            while (!have_decided_direction) {
                printf("Which side do you want to castle? ");
                std::ignore = scanf("%5s", nameofpiecetomove);
                clearinput();
                nameofpiecetomove[0] = toupper(nameofpiecetomove[0]);
                for (int i = 1; i < 5; i++) {
                    nameofpiecetomove[i] = tolower(nameofpiecetomove[i]);
                }
                if (strcmp(nameofpiecetomove, "Left") == 0) {
                    try {
                        castle_move = CastleMove(
                            Move(current_team->the_king.row, 5, current_team->the_king.row, 3,
                                &(current_team->the_king),
                                NULL),
                            &(current_team->rook1),
                            CastleDirection::LEFT, &mainboard, current_team);
                        have_decided_direction = true;
                        okmove = true;
                    }
                    catch (InvalidMove e) {
                        printf("%s\n", e.what());
                        okmove = false;
                        break;
                    }
                    catch (std::exception e) {
                        printf(e.what());
                        //You failed to castle.
                        okmove = false;
                        break;
                    }
                }
                else if (strcmp(nameofpiecetomove, "Right") == 0) {
                    try {
                        castle_move = CastleMove(
                            Move(current_team->the_king.row, 5, current_team->the_king.row, 7,
                                &(current_team->the_king),
                                NULL),
                            &(current_team->rook2),
                            RIGHT, &mainboard, current_team);
                        have_decided_direction = true;
                        okmove = true;
                    }
                    catch (InvalidMove e) {
                        printf("%s\n", e.what());
                        okmove = false;
                        break;
                    }
                    catch (std::exception e) {
                        printf(e.what());
                        //You failed to castle.
                        okmove = false;
                        break;
                    }
                }
                else if (strcmp(nameofpiecetomove, "Stop") == 0) {
                    okmove = false;
                    break;
                }
                else {
                    printf("Invalid direction. Try again.\n");
                }
            }

            //Still in the castling block and we know our move is legit if okmove is true.
            if (okmove) {
                try {
                    mainboard.human_move_piece(&castle_move);
                    /* NOTE: THE TEAMS SWAP ON THIS LINE.
                    * TODO: UPDATE THE BOARD'S KNOWLEDGE OF THE ENEMY TEAM'S CHECK HERE. */
                    current_team = current_team->enemy_team;
                }
                catch (InvalidMove e) {
                    printf("%s\n", e.what());
                    //You failed to move.
                    okmove = false;
                }
            }
            //Was that move safe? IF not, I am still in check.
            //*
            Game_Status am_I_still_in_check = mainboard.is_in_check(current_team->enemy_team, current_team, false);
            if (am_I_still_in_check != NEUTRAL) {
                printf("That's check, silly!\n");
                printf("Do you want to undo that move? Type Yes if so.\n");
                std::ignore = scanf("%3s", nameofpiecetomove);
                clearinput();
                nameofpiecetomove[0] = toupper(nameofpiecetomove[0]);
                for (int i = 1; i < 3; i++) {
                    nameofpiecetomove[i] = tolower(nameofpiecetomove[i]);
                }
                if (strcmp(nameofpiecetomove, "Yes") == 0) {
                    mainboard.undo_move(&castle_move, current_team);
                    current_team = current_team->enemy_team;
                }
            }
            /* UPDATE THE BOARD'S KNOWLEDGE OF THE BOTH TEAM'S CHECK HERE. */
            current_team->enemy_team->current_status = mainboard.is_in_check(current_team->enemy_team, current_team, false);
            current_team->current_status = mainboard.is_in_check(current_team, current_team->enemy_team, true);
            // END
            // */
        }

        //Find piece with that name
        piecefound = false;
        for (int i = 0; i < 8 && !did_try_tie && !did_try_castle && !do_load; i++) {
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
        } else if (!piecefound && !did_try_castle && !do_load) {
            printf("Invalid piece.\n");
        }
        
        
        //We found the piece. Now move it.
        bool landonokplace = false;
        bool enteredexactly1row = false;
        if (!wrong_team(piecetomove, current_team->color) && piecefound && !did_try_tie && !did_try_castle && !do_load) {
            printf("Where do you want to move %s?\n", nameofpiecetomove);
            printf("Enter your move.\n");
            char cspace = '\0';
            printf("Row: ");
            std::ignore = scanf("%c", &cspace);
            if (getchar() != '\n') {
                printf("Invalid row.\n");
                clearinput();
            }
            else {
                enteredexactly1row = true;
            }
            m_row = atoi(&cspace);
            if ((m_row >= 1) && (m_row <= 8) && enteredexactly1row) {
                bool enteredexactly1column = false;
                printf("Column: ");
                std::ignore = scanf("%c", &cspace);
                if (getchar() != '\n') {
                    printf("Invalid column.\n");
                    clearinput();
                }
                else {
                    enteredexactly1column = true;
                }

                m_column = atoi(&cspace);
                if (((m_column >= 1) && (m_column <= 8) && enteredexactly1column)) {
                    landonokplace = true;
                }
                else if (!landonokplace && enteredexactly1column) {
                    printf("Invalid column.\n");
                }
            }
            else if (!((m_row >= 1) && (m_row <= 8)) && enteredexactly1row) /*assume valid single int*/ {
                printf("Invalid row.\n");
            }
        }
        if (!wrong_team(piecetomove, current_team->color) && piecefound && !did_try_tie && !did_try_castle && !do_load && landonokplace) {

            bool should_upgrade_pawn = false;
            okmove = piecetomove->can_classmove(m_row, m_column, &mainboard);

            
            //Where we actually make the move
            Move tried_move;
            tried_move.end_row = m_row;
            tried_move.end_column = m_column;
            tried_move.piece_that_moved = piecetomove;
            tried_move.start_row = piecetomove->row;
            tried_move.start_column = piecetomove->column;
            if (okmove && mainboard.is_on_board(m_row, m_column)) {
                try {
                    /*IMPORTANT TO NOTE: This function can throw errors!
                      Also, it figures out on its own what piece was landed on. */
                    mainboard.human_move_piece(&tried_move);
                }
                catch (InvalidMove problem) {
                    printf(problem.what());
                    break;
                }
                catch (InvalidPiece problem) {
                    printf(problem.what());
                    break;
                }
                //Important: Upgrade the pawn if needed.
                if (piecetomove->piecetype == PAWN) {
                    upgrade_pawn_if_needed((Pawn*)piecetomove, current_team, &mainboard);
                }
                //NOTE: THE TEAMS SWAP ON THIS LINE
                current_team = current_team->enemy_team;
                did_try_castle = false;
                //Was that move safe? IF not, I am still in check.
                //*
                Game_Status am_I_still_in_check = mainboard.is_in_check(current_team->enemy_team, current_team, false);
                if (am_I_still_in_check != NEUTRAL) {
                    printf("That's check, silly!\n");
                    printf("Do you want to undo that move? Type Yes if so.\n");
                    std::ignore = scanf("%3s", nameofpiecetomove);
                    clearinput();
                    nameofpiecetomove[0] = toupper(nameofpiecetomove[0]);
                    for (int i = 1; i < 3; i++) {
                        nameofpiecetomove[i] = tolower(nameofpiecetomove[i]);
                    }
                    if (strcmp(nameofpiecetomove, "Yes") == 0) {
                        mainboard.undo_move(&tried_move, current_team);
                        current_team = current_team->enemy_team;
                    }
                }
                current_team->enemy_team->current_status = mainboard.is_in_check(current_team->enemy_team, current_team, false);
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
            else if(!did_try_castle) {
                if (did_try_tie) {
                    printf("Your opponent doen't want to quit yet.\n");
                }
                else if(!do_load) {
                    printf("Wrong team, silly.\n");
                }
            }
        }
        did_try_tie = false;
        if (do_load && !is_loaded) {
            is_loaded = true;
        }
        do_load = false;
    }
    if (whiteteam.the_king.row == blackteam.the_king.row
        && whiteteam.the_king.column == blackteam.the_king.column) {
        print_how_to_hug();
        std::ignore = scanf("%3s", nameofpiecetomove);
        nameofpiecetomove[0] = toupper(nameofpiecetomove[0]);
        clearinput();
        nameofpiecetomove[0] = toupper(nameofpiecetomove[0]);
        for (int i = 1; i < 3; i++) {
            nameofpiecetomove[i] = tolower(nameofpiecetomove[i]);
        }
        if (strcmp("Yes", nameofpiecetomove) == 0) {
            make_kings_hug(current_team->enemy_team, public_white_team, public_black_team);
            return 0;
        }
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

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file