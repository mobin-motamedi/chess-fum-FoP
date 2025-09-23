#ifndef VALIDATION_H
#define VALIDATION_H

#include "game_state.h"

void generatePieceMoves(GameState *state, int srcRow, int srcCol, char color);
void generateAllPieceMoves(GameState *state, char color);
void filterValidMoves(GameState *state,  char color);
int isValidMove(GameState *state, int srcRow, int srcCol, int destRow, int destCol);
int isUnderCheck(GameState *state, char color);

#endif
