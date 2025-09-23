#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "game_state.h"

#define MAX_CAPTURED 16

void updateBoard(GameState *state, int srcRow, int srcCol, int destRow, int destCol) {
    char piece = state->board[srcRow][srcCol];

    // Move the piece
    state->board[destRow][destCol] = piece;
    state->board[srcRow][srcCol] = '.';
}

// Function to get the score of a piece
int getPieceScore(char piece) {
    switch (piece) {
        case 'Q': case 'q': return 9;
        case 'R': case 'r': return 5;
        case 'B': case 'b': return 3;
        case 'N': case 'n': return 3;
        case 'P': case 'p': return 1;
        default: return 0;  // Default for any invalid piece
    }
}

// Function to print the board and captured pieces
void displayBoard(const GameState *state) {
    // Sort the captured pieces before printing
    char whiteCapturedSorted[MAX_CAPTURED];
    char blackCapturedSorted[MAX_CAPTURED];
    
    memcpy(whiteCapturedSorted, state->capturedWhite, sizeof(state->capturedWhite));
    memcpy(blackCapturedSorted, state->capturedBlack, sizeof(state->capturedBlack));

    // Sort both arrays of captured pieces based on their score
    for (int i = 0; i < MAX_CAPTURED - 1; i++) {
        for (int j = 0; j < MAX_CAPTURED - i - 1; j++) {
            if (getPieceScore(whiteCapturedSorted[j]) < getPieceScore(whiteCapturedSorted[j + 1])) {
                char temp = whiteCapturedSorted[j];
                whiteCapturedSorted[j] = whiteCapturedSorted[j + 1];
                whiteCapturedSorted[j + 1] = temp;
            }
        }
    }
    for (int i = 0; i < MAX_CAPTURED - 1; i++) {
        for (int j = 0; j < MAX_CAPTURED - i - 1; j++) {
            if (getPieceScore(blackCapturedSorted[j]) < getPieceScore(blackCapturedSorted[j + 1])) {
                char temp = blackCapturedSorted[j];
                blackCapturedSorted[j] = blackCapturedSorted[j + 1];
                blackCapturedSorted[j + 1] = temp;
            }
        }
    }

    // Print captured pieces at the top
    printf("\nCaptured by Black: ");
    for (int i = 0; i < MAX_CAPTURED; i++) {
        if (blackCapturedSorted[i] != '\0') {
            printf("%c ", blackCapturedSorted[i]);
        }
    }
    printf("\n\n");

    // Print the chessboard with column names and row numbers
    printf("  a b c d e f g h\n");
    for (int row = 0; row < 8; row++) {
        printf("%d ", 8 - row);
        for (int col = 0; col < 8; col++) {
            printf("%c ", state->board[row][col]);
        }
        printf("\n");
    }
    printf("  a b c d e f g h\n");

    // Print captured pieces at the bottom
    printf("\nCaptured by White: ");
    for (int i = 0; i < MAX_CAPTURED; i++) {
        if (whiteCapturedSorted[i] != '\0') {
            printf("%c ", whiteCapturedSorted[i]);
        }
    }
    printf("\n\n");
}
