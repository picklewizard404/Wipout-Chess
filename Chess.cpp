// Chess.cpp : This file contains the 'main' function. Program execution begins and ends there.
// I grant credit for Dad. He helped a lot.
#pragma warning(disable:4996)
#include "Chess_non_main.h"
#include "diagnoal_direction.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "..\Safety.h"
using namespace std;

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
    bool did_tie = false;
    //char current_team = 'w';
    Team* current_team = &whiteteam;

    TYPE type_of_piecetomove = PAWN;
    
    //Setup
    Piece* wKnight2 = whiteteam.pieces[6];
    Piece* bKnight2 = blackteam.pieces[6];
    Piece* wKing = whiteteam.pieces[4];
    Piece* bKing = blackteam.pieces[4];
    Piece* current_king = wKing;
    bool am_i_in_check = false;
    bool landed_on_king = false;
    printf("You can be killed.\n");

    //TEMP
    move_diagnolly(wKing, UP_LEFT, 1, &mainboard);

    // If playing
    //*
    while (wKing->alive && bKing->alive)
    {
        //say_pieces_of_team(&mainboard, current_team);
        mainboard.print_board();
        printf("%s turn.\n", team_name(current_team->color));
        if (current_team->color == WHITE) {
            //printf("White turn.\n");
            if (mainboard.is_in_check((King*)wKing, &blackteam, &mainboard)) {
                am_i_in_check = true;
                printf("You are in check!\n");
            }
        }
        else if (current_team->color == BLACK) {
            //printf("Black turn.\n");
            if (mainboard.is_in_check((King*)bKing, &whiteteam, &mainboard)) {
                am_i_in_check = true;
                printf("You are in check!\n");
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
            getchar();
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
                getchar();
                return 0;
            }
            else {
                did_tie = true;
            }
            
        }
        //Find piece with that name
        piecefound = false;
        for (int i = 0; i < 8 && !did_tie; i++) {
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
        if (!piecefound && !did_tie) {
            printf("Invalid piece.\n");
        }
        //We found the piece. Now move it.
        if (!wrong_team(piecetomove, current_team->color) && piecefound && !did_tie) {
            printf("Where do you want to move %s?\n", nameofpiecetomove);
            printf("Enter your move.\n");

            printf("Row: ");
            scanf("%d", &m_row);
            printf("Column: ");
            scanf("%d", &m_column);
            

            
            switch (type_of_piecetomove)
            {
            case PAWN:
                //If pawn is at the end:
                for (int i = 0; i < 16; i++) {
                    if (strcmp(nameofpiecetomove, current_team->pieces[i]->name) == 0) {
                        //TODO: Upgrade current_team->pieces[i]
                    }
                }
                break;
            case ROOK:
                okmove = ((Rook*)piecetomove)->can_classmove(m_row, m_column, &mainboard);
                break;
            case KNIGHT:
                okmove = ((Knight*)piecetomove)->can_classmove(m_row, m_column, &mainboard);
                break;
            case BISHOP:
                break;
            case QUEEN:
                break;
            case KING:
                okmove = ((King*)piecetomove)->can_kingmove(m_row, m_column, &mainboard, &landed_on_king);
                if (landed_on_king) {
                    printf("The kings hugged. It's a happy tie!\n");
                    scanf("%1s", nameofpiecetomove);
                    return 0;
                }
                break;
            default:
                saybadmove();
            }

            
            if (okmove && mainboard.is_on_board(m_row, m_column)) {
                
                //If we get to this line, we know the move is valid and if this function returns True,
                // It's a CHECKMATE and we win.
                
                if (do_move(&current_team, &mainboard, piecetomove, m_row, m_column, public_white_team, public_black_team)) {
                    //checkmate!
                    printf("%s team wins.", current_team->full_name);
                    scanf("%1s", nameofpiecetomove);
                    return 0;
                }
            }

            else {
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
                if (did_tie) {
                    printf("Your opponent doen't want to quit yet.\n");
                }
                else {
                    printf("Wrong team, silly.\n");
                }
            }
        }
        did_tie = false;
    }

    if (!bKing->alive) {
        printf("White wins!\n");
    }
    if (!wKing->alive) {
        printf("Black wins!\n");
    }
    scanf("%9s", nameofpiecetomove);
}

struct {
    bool whitealive;
    bool blackalive;
    bool did_hug;
} Chess_hug;
bool make_kings_hug() {
    Board mainboard;
    Team whiteteam = Team(WHITE, &mainboard);
    Team blackteam = Team(BLACK, &mainboard);
    whiteteam.enemy_team = &blackteam;
    blackteam.enemy_team = &whiteteam;
    mainboard.place(&(blackteam.the_king), 2, 5);
    Chess_hug.did_hug = false;
    Chess_hug.blackalive = blackteam.the_king.alive;
    Chess_hug.whitealive = whiteteam.the_king.alive;
    mainboard.move(&(whiteteam.the_king), 2, 5, &(Chess_hug.did_hug));
    return Chess_hug.did_hug;
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