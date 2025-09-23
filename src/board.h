#ifndef BOARD_H
#define BOARD_H

#include "game_state.h"

void displayBoard(const GameState *state);
void updateBoard(GameState *state, int srcRow, int srcCol, int destRow, int destCol);

#endif // BOARD_H
