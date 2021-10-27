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
*       AI opponent declaration
*
********************************************************************************************/

#ifndef AIOPPONENT_H
#define AIOPPONENT_H

#include "def.h"
#include "movelist.h"
#include "game.h"

// 24 bytes
typedef struct AIOpponent {
    MoveList* moves;// list of moves possible
    float eval;     // score of chosen spot
    int8_t depth;   // depth chosen to search
    int8_t x;       // chosen x spot to place
    int8_t y;       // chosen y spot to place
    int8_t stage;   // moves played this game
    bool p1;        // if ai is player 1
} AIOpponent;

// Initializes AI for use
// ai: ai to initiallize
// @Return: true on success, false on failure
bool AI_Init(AIOpponent** ai);

// Cleans up ai and frees memory
// ai: ai to free
void AI_Free(AIOpponent* ai);

// Chooses a move generated from minimax
// ai: ai struct to use
// board: the board state to use
// @Return: bool if move was selected or not
bool AI_Choose(AIOpponent* ai, int8_t board[][BOARD_SIDE]);

/*******************************************************************************************
* Move Selection Support
********************************************************************************************/
// Generates score of the board state after piece has been placed
// player1: if evaluating for player 1
// stage: how many turns have elapsed
// tempboard: what the board will look like after the play
// @Return: score of the board state
float EvaluateBoardState(bool player1, int8_t stage, int8_t tempboard[][BOARD_SIDE]);

// Creates a tree of possible board moves and results
// depth: Recursive depth count
// alpha: max value to watch for
// beta: min valaue to watch for
// stage: how many moves have been made by AI
// maxer: if evaluating for a maxer
// player1: if looking at a player1
// tempboard: a temp board state
// @Return: the evaluation score
float Search(int8_t depth, float alpha, float beta, int8_t stage, bool maxer, bool player1, int8_t tempboard[][BOARD_SIDE]);

// Checks if the piece is a stable piece(symmetrical on all sides) so it can't be flipped
// x: x location
// y: y location
// tempboard: a temp board state
//@Return: true if it's a stable piece
bool IsStable(int8_t x, int8_t y, int8_t tempboard[][BOARD_SIDE]);

// Gets the number of tokens surrounding a location
// x: x location 
// y: y location
// tempboard: a temp board state
//@Return: number of tokens surrounding a piece
int NumberOfTokensAround(int8_t x, int8_t y, int8_t tempboard[][BOARD_SIDE]);

// Counts how many possible moves there are for a player
// player1: player to look for moves
// tempboard: a temp board state
// @Return: number of possible moves
int PossibleMovesCount(bool player1, int8_t tempboard[][BOARD_SIDE]);

// Checks if the piece is a frontier piece
// x: x location 
// y: y location
// tempboard: a temp board state
// @Return: true if it's a frontier piece
bool IsFrontier(int8_t x, int8_t y, int8_t tempboard[][BOARD_SIDE]);

// Total possible piece captures
// x: start x
// y: start y
// @Return: number of pieces to capture in all directions
int8_t TotalPossibleCapture(int8_t x, int8_t y, bool player1, int8_t board[][BOARD_SIDE]);

// Checks if there are any pieces around it that can be flipped
// x: center x position
// y: center y position
// player1: if we're checking for player1
// tempboard: board to check
// @Return true if there is an opponent token
bool HasTargetsAround(int8_t x, int8_t y, bool player1, int8_t tempboard[][BOARD_SIDE]);

#endif //AIOPPONENT_H