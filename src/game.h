/*******************************************************************************************
*
*   Reversi AI
*
*   This game has been created using raylib 3.7 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Derek Nguyen
*
*   DESCRIPTION:
*       Main game data structure
*
********************************************************************************************/

#ifndef GAME_H
#define GAME_H

#include "def.h"
typedef struct Game {
    bool P1Turn;
    int8_t P1Score;
    int8_t P2Score;
    int8_t Board[BOARD_SIDE][BOARD_SIDE];
}Game;

// Init the game
// game: game to initialize
// @Return: true if able to init game
bool InitGame(Game** game);

// places the 4 pieces in the middle
// board: board to initialize
void InitBoard(int8_t board[][BOARD_SIDE]);

// Copies board onto the new board
// board: board to copy
// newboard: target board
void CopyBoard(int8_t board[][BOARD_SIDE], int8_t newboard[][BOARD_SIDE]);

// Resets game and board to initial state
// game: game to reset
void ResetGame(Game* game);

// Frees the game memory
// game: game to free
void FreeGame(Game* game);

// Checks if the game is over(if all spaces are taken, or one side has 0 tokens)
// game: game to check
// @Return: true if game is over
bool CheckGameOver(Game* game);

// Places the piece and then sums up points(flip happens before this)
// game: game to commit to
void CommitMove(Game* game);

// Checks if there are any pieces around it that can be flipped
// x: center x position
// y: center y position
// player1: if we're checking for player1
// tempboard: board to check
// @Return true if there is an opponent token
bool HasTargetsAround(int8_t x, int8_t y, bool player1, int8_t tempboard[][BOARD_SIDE]);

// Number of pieces that can be captured in a direction
// x: x of the first capture piece
// y: y of the first capture piece
// dx: direction to move towards x
// dy: direction to move towards y
// @Return: number of pieces that can be captured
int8_t PiecesInDirection(int8_t x, int8_t y, int8_t dx, int8_t dy, bool player1, int8_t board[][BOARD_SIDE]);



// Flips pieces on the board
// x: start x
// y: start y
// player1: if flipping for player1
// Return: true If any pieces were flipped
bool FlipPieces(const int8_t x, const int8_t y, bool player1, int8_t board[][BOARD_SIDE]);

#endif //GAME_H
