#include "game_state.h"
#include "board.h"
#include "validation.h"
#include "game.h"
#include <stdio.h>
#include <string.h>

void initializeGame(GameState *state) {
    // initialize board
    char initialBoard[8][8] = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    };
    
    memcpy(state->board, initialBoard, sizeof(initialBoard));
    state->currentTurn = 'w';  // White's move first
    state->capturedWhiteCount = 0;
    state->capturedBlackCount = 0;
    memset(state->capturedWhite, 0, sizeof(state->capturedWhite));
    memset(state->capturedBlack, 0, sizeof(state->capturedBlack));
    state->pieceMovesCountWhite = 0;
    state->pieceMovesCountBlack = 0;
    state->validMovesCountWhite = 0;
    state->validMovesCountBlack = 0;
}

int main(void) {
    GameState state;
    initializeGame(&state);

    while (1) {
        displayBoard(&state);
        if (isUnderCheck(&state, state.currentTurn)) {
            printf("%s's in Check!\n", (state.currentTurn == 'w') ? "White" : "Black");
        }
        char input[5];
        printf("%s's move: ", (state.currentTurn == 'w') ? "White" : "Black");
        scanf("%4s", input);
        
        if (!processMove(&state, input)) {
            continue;
        }
        if (isGameOver(&state)) break;
    }
    displayBoard(&state);
    printf("Game Over!\n");
    return 0;
}
