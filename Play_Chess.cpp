#include "Chess_code\Chess.h"
#include <stdio.h>
#include <time.h>
#include <chrono>
#include <thread>
#include "Chess_code\Chess.h"
//Main game
int main(int argc, char*argv[]) {
	// There are command line args now.
	bool talkhug = false;
	if (argc > 1) {
		if (strcmp(argv[1], "/?") == 0) {
			printf("Command line arguments:\n--hug: Learn how to help the kins hug!");
		}
		for (int i = 1; i < argc; i++) {
			if (strcmp("--hug", argv[i]) == 0) {
				talkhug = true;
			}
		}
	}
	printf("I'll tell you this exactly once:\nThe top right square is row 8 column 8 and the bottom left square is row 1 column 1.\n");
	printf("You can be killed.");
	if (talkhug) {
		printf(" Or you can hug instead. ");
		printf("If you type the word hug instad of moving a piece, you get a happy ending.\nThe kings hug!\n");
	}
	else {
		printf("\n");
	}
	
	chess();
	sleep5();
	
	return 0;
}