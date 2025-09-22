#include "game_state.h"
#include "game.h"
#include "validation.h"
#include "board.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void switchTurn(GameState *state) {
    state->currentTurn = (state->currentTurn == 'w') ? 'b' : 'w';
}

void handlePawnPromotion(GameState *state, int destRow, int destCol, char color) {
    char piece = state->board[destRow][destCol];
    if ((color == 'w' && destRow == 0 && piece == 'P') ||
        (color == 'b' && destRow == 7 && piece == 'p')) {
        char promotedPiece;
        printf("Pawn promotion! Choose a piece (Q, R, B, N): ");
        scanf(" %c", &promotedPiece);

        if (color == 'w') {
            promotedPiece = toupper(promotedPiece);
        } else {
            promotedPiece = tolower(promotedPiece);
        }

        while (promotedPiece != 'Q' && promotedPiece != 'R' && promotedPiece != 'B' && promotedPiece != 'N' &&
               promotedPiece != 'q' && promotedPiece != 'r' && promotedPiece != 'b' && promotedPiece != 'n') {
            printf("Invalid choice! Choose again (Q, R, B, N): ");
            scanf(" %c", &promotedPiece);
            if (color == 'w') {
                promotedPiece = toupper(promotedPiece);
            } else {
                promotedPiece = tolower(promotedPiece);
            }
        }

        state->board[destRow][destCol] = promotedPiece;
    }
}

void makeMove(GameState *state, int srcRow, int srcCol, int destRow, int destCol) {
    char targetPiece = state->board[destRow][destCol];

    // Capture logic
    if (targetPiece != '.') {
        if (state->currentTurn == 'w' && targetPiece >= 'a' && targetPiece <= 'z') {
            for (int i = 0; i < MAX_CAPTURED; i++) {
                if (state->capturedWhite[i] == '\0') {
                    state->capturedWhite[i] = targetPiece;
                    state->capturedWhiteCount++;
                    break;
                }
            }
        } else if (state->currentTurn == 'b' && targetPiece >= 'A' && targetPiece <= 'Z') {
            for (int i = 0; i < MAX_CAPTURED; i++) {
                if (state->capturedBlack[i] == '\0') {
                    state->capturedBlack[i] = targetPiece;
                    state->capturedBlackCount++;
                    break;
                }
            }
        }
    }

    // Update the board
    updateBoard(state, srcRow, srcCol, destRow, destCol);

    // Handle pawn promotion
    handlePawnPromotion(state, destRow, destCol, state->currentTurn);

    // Switch turns
    switchTurn(state);
}

int processMove(GameState *state, const char *input) {
    // Check if the input length is exactly 4 characters
    if (strlen(input) != 4) {
        printf("Invalid input format. Use format like e2e4.\n");
        return 0;
    }
    
    // Convert input characters to board coordinates
    int srcCol = input[0] - 'a';
    int srcRow = 8 - (input[1] - '0');
    int destCol = input[2] - 'a';
    int destRow = 8 - (input[3] - '0');
       
    // Validate the move coordinates
    if (srcRow < 0 || srcRow >= 8 || srcCol < 0 || srcCol >= 8 ||
        destRow < 0 || destRow >= 8 || destCol < 0 || destCol >= 8) {
        printf("Invalid move coordinates.\n");
        return 0;
    }
    
    // Generate and filter player's moves
    generateAllPieceMoves(state, state->currentTurn);
    filterValidMoves(state, state->currentTurn);
    
    // Check if the move is valid according to the game rules
    if (!isValidMove(state, srcRow, srcCol, destRow, destCol)) {
        printf("Invalid move.\n");
        return 0;
    }
    
    // Make the move and update the game state
    makeMove(state, srcRow, srcCol, destRow, destCol);
    return 1;
}

int isGameOver(GameState *state) {
    char currentTurn = state->currentTurn;
    char opponentColor = (currentTurn == 'w') ? 'b' : 'w';

    // Check if the current player's king is under check
    if (isUnderCheck(state, currentTurn)) {
        // Generate all valid moves for the current player
        generateAllPieceMoves(state, currentTurn);
        filterValidMoves(state, currentTurn);
        Move *validMoves = (currentTurn == 'w') ? state->allValidMovesWhite : state->allValidMovesBlack;
        int moveCount = (currentTurn == 'w') ? state->validMovesCountWhite : state->validMovesCountBlack;

        // If no valid moves are available, it's checkmate
        if (moveCount == 0) {
            printf("Checkmate! %s wins.\n", (opponentColor == 'w') ? "White" : "Black");
            return 1;
        }
    } else {
        // Generate all valid moves for the current player
        generateAllPieceMoves(state, currentTurn);
        filterValidMoves(state, currentTurn);
        Move *validMoves = (currentTurn == 'w') ? state->allValidMovesWhite : state->allValidMovesBlack;
        int moveCount = (currentTurn == 'w') ? state->validMovesCountWhite : state->validMovesCountBlack;

        // If no valid moves are available, it's stalemate
        if (moveCount == 0) {
            printf("Stalemate! It's a draw.\n");
            return 1;
        }

        // Check for insufficient material
        if (isInsufficientMaterial(state)) {
            printf("Insufficient material! It's a draw.\n");
            return 1;
        }
    }

    // The game is not over
    return 0;
}

int isInsufficientMaterial(GameState *state) {
    int whiteBishops = 0, whiteKnights = 0, whiteOthers = 0;
    int blackBishops = 0, blackKnights = 0, blackOthers = 0;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            char piece = state->board[row][col];
            switch (piece) {
                case 'B': whiteBishops++; break;
                case 'N': whiteKnights++; break;
                case 'P': case 'R': case 'Q': whiteOthers++; break;
                case 'b': blackBishops++; break;
                case 'n': blackKnights++; break;
                case 'p': case 'r': case 'q': blackOthers++; break;
            }
        }
    }

    // King vs. King
    if (whiteBishops == 0 && whiteKnights == 0 && whiteOthers == 0 &&
        blackBishops == 0 && blackKnights == 0 && blackOthers == 0) {
        return 1;
    }

    // King and Bishop vs. King
    if (whiteBishops == 1 && whiteKnights == 0 && whiteOthers == 0 &&
        blackBishops == 0 && blackKnights == 0 && blackOthers == 0) {
        return 1;
    }
    if (whiteBishops == 0 && whiteKnights == 0 && whiteOthers == 0 &&
        blackBishops == 1 && blackKnights == 0 && blackOthers == 0) {
        return 1;
    }

    // King and Knight vs. King
    if (whiteBishops == 0 && whiteKnights == 1 && whiteOthers == 0 &&
        blackBishops == 0 && blackKnights == 0 && blackOthers == 0) {
        return 1;
    }
    if (whiteBishops == 0 && whiteKnights == 0 && whiteOthers == 0 &&
        blackBishops == 0 && blackKnights == 1 && blackOthers == 0) {
        return 1;
    }

    // King and Bishop vs. King and Bishop (same color)
    if (whiteBishops == 1 && whiteKnights == 0 && whiteOthers == 0 &&
        blackBishops == 1 && blackKnights == 0 && blackOthers == 0) {
        // Check if both bishops are on the same color
        // Assuming the board is represented as an 8x8 array with (0,0) being a white square
        int whiteBishopColor = (state->board[0][0] == 'B') ? 1 : 0;
        int blackBishopColor = (state->board[0][0] == 'b') ? 1 : 0;
        if (whiteBishopColor == blackBishopColor) {
            return 1;
        }
    }

    return 0;
}