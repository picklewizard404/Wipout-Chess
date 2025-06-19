#include "Chess_code\Chess.h"
#include <stdio.h>
#include <time.h>
#include <chrono>
#include <thread>
//Main game
int main(int argc, char*argv[]) {
    // There are command line args now.
    bool talkhug = false;
    bool should_load_man = false;
    bool should_load_save = false;
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (strcmp("--hug", argv[i]) == 0) {
                talkhug = true;
            }
            if (strcmp(argv[i], "/?") == 0 || strcmp(argv[i], "--help") == 0) {
                printf("--help or /?: View these help messages.\n");
                printf("--loadman: MANUALLY Set the game in its prior state.\n");
                printf("In-game command load: Load the file from an automatically saved game.\n");
                printf("->To save a game, type save\n");
                printf("->Your game will be saved in the same directory as this exe with the name Saved_Game.chess\n");
                printf("> To say you're done setting up, type cteam to start as the current team...\n...  or oteam to start as the enemy team.\n");
                printf("> I recommend noting which movement was the last one you made, and taking a screenshot.\n");
                printf("Space numbering:\nThe top right square is row 8 column 8 and the bottom left square is row 1 column 1.\n");
                printf("> Note that when I ask for the row and column,\n> I read the numbers from a top-down perspective with the white team on the bottom.\n");
                printf("> When you have to enter a row or column, you must be very precise.\n> You can't type anything after the single number character.\n");
                printf("Pro type for black team:\n> To figure out the column of your pawn, (assuming it's in its starting column),\n> subtract the number in its name from 9.\n");
                printf("When castling is a legal move for you, you can castle by typing the word castle with no punctuation.\n");
                printf("> The game will ask you whether you want to castle left or right.\n");
                printf("> Castling left always involves the rook in column 1,\n> and castling right always involves the rook in column 8.\n");
                printf("You can be killed. Or you can hug!\n");
                printf("You can surrender by typing the word surrender.\n");
                printf("Or if your opponent agrees, you can tie by typing the word tie.\n");
                printf("Or you can make kings hug by helping them land on the same space and meet,\n> or by typing the word hug instead of selecting a piece.\n");
                printf("Command line arguments:\n--hug: Display that the kings can hug when starting the actual game!\n");
                printf(">  Hugging is a secret move. Think of it as a reward for being nice.\n");
                printf("God answered my prayers and helped me make this game. He deserves credit!\n");
                return 0;
            }
            if (strcmp("--loadman", argv[i]) == 0) {
                should_load_man = true;
            }
            if (strcmp("--loadsave", argv[i]) == 0) {
                should_load_save = true;
                if (i < argc - 1) {
                    printf("Error: No savefile name.");
                }
               
            }
        }
        if (talkhug) {
            printf("You can make kings hug by helping them land on the same space and meet, or by typing the word hug instead of selecting a piece.\n");
        }
    }
    if (should_load_man && should_load_save) {
        printf("Do you want to load from a save file or re-create the game by hand?\nMake up your mind!\n");
        return 2;
    }
    printf("Note that when I ask for the row and column,\nI read the numbers from a top-down perspective with the white team on the bottom.\n");
    printf("I'll tell you this exactly once:\nThe top right square is row 8 column 8 and the bottom left square is row 1 column 1.\n");
    printf("When you have to enter a row or column, you must be very precise.\nYou can't type anything after the single number character.\n");
    printf("You can be killed.\n");
    
    chess(should_load_man);
    sleep5();
    
    return 0;
}