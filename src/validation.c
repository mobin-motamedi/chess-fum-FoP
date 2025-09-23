#include "game_state.h"
#include "validation.h"
#include "board.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void generateAllPieceMoves(GameState *state, char color) {
    if (color == 'w') {
        state->pieceMovesCountWhite = 0;
    } else {
        state->pieceMovesCountBlack = 0;
    }

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            char piece = state->board[row][col];
            if (color == 'w' && piece >= 'A' && piece <= 'Z') {
                generatePieceMoves(state, row, col, 'w');
            } else if (color == 'b' && piece >= 'a' && piece <= 'z') {
                generatePieceMoves(state, row, col, 'b');
            }
        }
    }
}

void filterValidMoves(GameState *state, char color) {
    if (color == 'w') {
        state->validMovesCountWhite = 0;

        // Process white moves
        for (int i = 0; i < state->pieceMovesCountWhite; i++) {
            GameState tempState = *state;
            updateBoard(&tempState, state->allPieceMovesWhite[i].srcRow, state->allPieceMovesWhite[i].srcCol,
                        state->allPieceMovesWhite[i].destRow, state->allPieceMovesWhite[i].destCol);

            if (!isUnderCheck(&tempState, 'w')) {  // Ensure move doesn't put white in check
                state->allValidMovesWhite[state->validMovesCountWhite] = state->allPieceMovesWhite[i];
                state->validMovesCountWhite++;
            }
        }
    } else {
        state->validMovesCountBlack = 0;

        // Process black moves
        for (int i = 0; i < state->pieceMovesCountBlack; i++) {
            GameState tempState = *state;
            updateBoard(&tempState, state->allPieceMovesBlack[i].srcRow, state->allPieceMovesBlack[i].srcCol,
                        state->allPieceMovesBlack[i].destRow, state->allPieceMovesBlack[i].destCol);

            if (!isUnderCheck(&tempState, 'b')) {  // Ensure move doesn't put black in check
                state->allValidMovesBlack[state->validMovesCountBlack] = state->allPieceMovesBlack[i];
                state->validMovesCountBlack++;
            }
        }
    }
}

int isValidMove(GameState *state, int srcRow, int srcCol, int destRow, int destCol) {
    Move *validMoves = (state->currentTurn == 'w') ? state->allValidMovesWhite : state->allValidMovesBlack;
    int moveCount = (state->currentTurn == 'w') ? state->validMovesCountWhite : state->validMovesCountBlack;
    for (int i = 0; i < moveCount; i++) {
        if (validMoves[i].srcRow == srcRow && validMoves[i].srcCol == srcCol &&
            validMoves[i].destRow == destRow && validMoves[i].destCol == destCol) {
            return 1;
        }
    }
    return 0;
}


int isUnderCheck(GameState *state, char color) {
    int kingRow = -1, kingCol = -1;
    char king = (color == 'w') ? 'K' : 'k';

    // Find the king's position
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (state->board[row][col] == king) {
                kingRow = row;
                kingCol = col;
                break;
            }
        }
        if (kingRow != -1) break; // Exit outer loop if king is found
    }
    if (kingRow == -1 || kingCol == -1) return 1; // King not found (invalid state)
    
    char opponentColor = (color == 'w') ? 'b' : 'w';
    GameState tempState = *state;
    generateAllPieceMoves(&tempState, opponentColor);

    Move *opponentMoves = (opponentColor == 'w') ? tempState.allPieceMovesWhite : tempState.allPieceMovesBlack;
    int moveCount = (opponentColor == 'w') ? tempState.pieceMovesCountWhite : tempState.pieceMovesCountBlack;

    // Check if any opponent move can capture the king
    for (int i = 0; i < moveCount; i++) {
        if (opponentMoves[i].destRow == kingRow && opponentMoves[i].destCol == kingCol)
            return 1;
    }
    return 0;
}



void generatePawnMoves(GameState *state, int srcRow, int srcCol, char color) {
    int direction = (color == 'w') ? -1 : 1;
    int startRow = (color == 'w') ? 6 : 1;
    Move *moves = (color == 'w') ? state->allPieceMovesWhite : state->allPieceMovesBlack;
    int *count = (color == 'w') ? &state->pieceMovesCountWhite : &state->pieceMovesCountBlack;
    
    // Move one square forward
    if (state->board[srcRow + direction][srcCol] == '.')
        moves[(*count)++] = (Move){srcRow, srcCol, srcRow + direction, srcCol};
    
    // Move two squares forward from start position
    if (srcRow == startRow && state->board[srcRow + direction][srcCol] == '.' &&
        state->board[srcRow + 2 * direction][srcCol] == '.')
        moves[(*count)++] = (Move){srcRow, srcCol, srcRow + 2 * direction, srcCol};
    
    // Capture diagonally
    for (int dCol = -1; dCol <= 1; dCol += 2) {
        int captureCol = srcCol + dCol;
        if (captureCol >= 0 && captureCol < 8) {
            char target = state->board[srcRow + direction][captureCol];
            if (target != '.' && ((color == 'w' && target >= 'a' && target <= 'z') ||
                                  (color == 'b' && target >= 'A' && target <= 'Z')))
                moves[(*count)++] = (Move){srcRow, srcCol, srcRow + direction, captureCol};
        }
    }
}

void generateRookMoves(GameState *state, int srcRow, int srcCol, char color) {
    Move *moves = (color == 'w') ? state->allPieceMovesWhite : state->allPieceMovesBlack;
    int *count = (color == 'w') ? &state->pieceMovesCountWhite : &state->pieceMovesCountBlack;

    // Horizontal and vertical moves
    for (int d = -1; d <= 1; d += 2) {
        for (int i = 1; i < 8; i++) {
            int newRow = srcRow + i * d;
            int newCol = srcCol;
            if (newRow >= 0 && newRow < 8) {
                char target = state->board[newRow][newCol];
                if (target == '.') {
                    moves[(*count)++] = (Move){srcRow, srcCol, newRow, newCol};
                } else {
                    if ((color == 'w' && target >= 'a' && target <= 'z') ||
                        (color == 'b' && target >= 'A' && target <= 'Z')) {
                        moves[(*count)++] = (Move){srcRow, srcCol, newRow, newCol};
                    }
                    break;
                }
            }
        }
        for (int i = 1; i < 8; i++) {
            int newRow = srcRow;
            int newCol = srcCol + i * d;
            if (newCol >= 0 && newCol < 8) {
                char target = state->board[newRow][newCol];
                if (target == '.') {
                    moves[(*count)++] = (Move){srcRow, srcCol, newRow, newCol};
                } else {
                    if ((color == 'w' && target >= 'a' && target <= 'z') ||
                        (color == 'b' && target >= 'A' && target <= 'Z')) {
                        moves[(*count)++] = (Move){srcRow, srcCol, newRow, newCol};
                    }
                    break;
                }
            }
        }
    }
}

void generateKnightMoves(GameState *state, int srcRow, int srcCol, char color) {
    Move *moves = (color == 'w') ? state->allPieceMovesWhite : state->allPieceMovesBlack;
    int *count = (color == 'w') ? &state->pieceMovesCountWhite : &state->pieceMovesCountBlack;
    int knightMoves[8][2] = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}};

    for (int i = 0; i < 8; i++) {
        int newRow = srcRow + knightMoves[i][0];
        int newCol = srcCol + knightMoves[i][1];
        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
            char target = state->board[newRow][newCol];
            if (target == '.' || (color == 'w' && target >= 'a' && target <= 'z') ||
                (color == 'b' && target >= 'A' && target <= 'Z')) {
                moves[(*count)++] = (Move){srcRow, srcCol, newRow, newCol};
            }
        }
    }
}

void generateBishopMoves(GameState *state, int srcRow, int srcCol, char color) {
    Move *moves = (color == 'w') ? state->allPieceMovesWhite : state->allPieceMovesBlack;
    int *count = (color == 'w') ? &state->pieceMovesCountWhite : &state->pieceMovesCountBlack;

    // Diagonal moves
    for (int dRow = -1; dRow <= 1; dRow += 2) {
        for (int dCol = -1; dCol <= 1; dCol += 2) {
            for (int i = 1; i < 8; i++) {
                int newRow = srcRow + i * dRow;
                int newCol = srcCol + i * dCol;
                if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                    char target = state->board[newRow][newCol];
                    if (target == '.') {
                        moves[(*count)++] = (Move){srcRow, srcCol, newRow, newCol};
                    } else {
                        if ((color == 'w' && target >= 'a' && target <= 'z') ||
                            (color == 'b' && target >= 'A' && target <= 'Z')) {
                            moves[(*count)++] = (Move){srcRow, srcCol, newRow, newCol};
                        }
                        break;
                    }
                }
            }
        }
    }
}

void generateQueenMoves(GameState *state, int srcRow, int srcCol, char color) {
    generateRookMoves(state, srcRow, srcCol, color);
    generateBishopMoves(state, srcRow, srcCol, color);
    generateKnightMoves(state, srcRow, srcCol, color);
}

void generateKingMoves(GameState *state, int srcRow, int srcCol, char color) {
    Move *moves = (color == 'w') ? state->allPieceMovesWhite : state->allPieceMovesBlack;
    int *count = (color == 'w') ? &state->pieceMovesCountWhite : &state->pieceMovesCountBlack;
    int kingMoves[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

    for (int i = 0; i < 8; i++) {
        int newRow = srcRow + kingMoves[i][0];
        int newCol = srcCol + kingMoves[i][1];
        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
            char target = state->board[newRow][newCol];
            if (target == '.' || (color == 'w' && target >= 'a' && target <= 'z') ||
                (color == 'b' && target >= 'A' && target <= 'Z')) {
                moves[(*count)++] = (Move){srcRow, srcCol, newRow, newCol};
            }
        }
    }
}

void generatePieceMoves(GameState *state, int srcRow, int srcCol, char color) {
    char piece = state->board[srcRow][srcCol];
    switch (piece) {
        case 'P': case 'p': generatePawnMoves(state, srcRow, srcCol, color); break;
        case 'R': case 'r': generateRookMoves(state, srcRow, srcCol, color); break;
        case 'N': case 'n': generateKnightMoves(state, srcRow, srcCol, color); break;
        case 'B': case 'b': generateBishopMoves(state, srcRow, srcCol, color); break;
        case 'Q': case 'q': generateQueenMoves(state, srcRow, srcCol, color); break;
        case 'K': case 'k': generateKingMoves(state, srcRow, srcCol, color); break;
    }
}
