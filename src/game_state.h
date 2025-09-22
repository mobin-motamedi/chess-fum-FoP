#ifndef GAME_STATE_H
#define GAME_STATE_H

#define MAX_CAPTURED 16
#define MAX_MOVES 200

typedef struct {
    int srcRow, srcCol;
    int destRow, destCol;
} Move;

typedef struct {
    char board[8][8];
    char capturedWhite[MAX_CAPTURED];
    char capturedBlack[MAX_CAPTURED];
    int capturedWhiteCount;
    int capturedBlackCount;
    char currentTurn;
    Move allPieceMovesWhite[MAX_MOVES];
    Move allPieceMovesBlack[MAX_MOVES];
    Move allValidMovesWhite[MAX_MOVES];
    Move allValidMovesBlack[MAX_MOVES];
    int pieceMovesCountWhite;
    int pieceMovesCountBlack;
    int validMovesCountWhite;
    int validMovesCountBlack;
} GameState;

#endif