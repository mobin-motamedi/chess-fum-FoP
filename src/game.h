#ifndef GAME_H
#define GAME_H

#include "game_state.h"

void handlePawnPromotion(GameState *state, int destRow, int destCol, char color);
void makeMove(GameState *state, int srcRow, int srcCol, int destRow, int destCol);
int processMove(GameState *state, const char *input);
int isGameOver(GameState *state);
int isInsufficientMaterial(GameState *state);
#endif
