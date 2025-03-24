#include "Chess_code\Chess.h"
#include <stdio.h>
#include <time.h>
#include <chrono>
#include <thread>
//Main game
int main(int argc, char*argv[]) {
    // There are command line args now.
    bool talkhug = false;
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (strcmp("--hug", argv[i]) == 0) {
                talkhug = true;
            }
            if (strcmp(argv[i], "/?") == 0 || strcmp(argv[i], "--help") == 0) {
                printf("Command line arguments:\n--hug: Learn how to help the kings hug!\n");
                printf("--help or /?: View these help messages.\n");
                printf("Space numbering:\nThe top right square is row 8 column 8 and the bottom left square is row 1 column 1.\n");
                printf("You can be killed. Or you can hug!\n");
                printf("God answered my prayers and helped me make this game. He deserves credit!\n");
                return 0;
            }
        }
        if (talkhug) {
            printf("You can make kings hug by helping them land on the same space and meet, or by typing the word hug instead of selecting a piece.\n");
        }
    }
    printf("I'll tell you this exactly once:\nThe top right square is row 8 column 8 and the bottom left square is row 1 column 1.\n");
    printf("You can be killed.\n");
    
    chess();
    sleep5();
    
    return 0;
}