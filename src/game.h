/* date = October 22nd 2021 6:33 pm */

#ifndef GAME_H
#define GAME_H

#include "def.h"
#include "aiopponent"

typedef struct Game {
    bool P1Turn = true;
    int P1Score = 0;
    int P2Score = 0;
    AIOpponent* aiopponent;
}Game;

// Gameplay Functions
//-----------------------------------------------------------------------------------------
//Determines which color piece to place based on player turn
//@Return: the piece to place
short PlayerPiece();

//Determines which color piece to place based on opponent turn
//@Return: the piece to place
short OpponentPiece();

//Checks if it's the player's turn
//@Return true if player turn matches player
bool IsPlayerTurn();

//Checks if there are any pieces around it that can be flipped
//x: center x position
//y: center y position
//player1: if we're checking for player1
//tempboard: board to check
//@Return true if there is an opponent token
bool HasTargetsAround(short x, short y, bool player1, short tempboard[][BOARD_SIDE]);

// Number of pieces that can be captured in a direction
//x: x of the first capture piece
//y: y of the first capture piece
//dx: direction to move towards x
//dy: direction to move towards y
//Return: number of pieces that can be captured
int PiecesInDirection(short x, short y, short dx, short dy, bool player1, short board[][BOARD_SIDE]);

// Total possible piece captures
//x: start x
//y: start y
//@Return: number of pieces to capture in all directions
int TotalPossibleCapture(short x, short y, bool player1, short board[][BOARD_SIDE]);

//
//x: start x
//y: start y
//Return: If any pieces were flipped
int FlipPieces(const short x, const short y, bool player1, short board[][BOARD_SIDE]);

//Init the board
void InitBoard(short board[][BOARD_SIDE]);

void CopyBoard(short board[][BOARD_SIDE], short newboard[][BOARD_SIDE]);

//Resets the game
void ResetGame(short board[][BOARD_SIDE]);

//Determines the winner and ends the game
void EndGame();

#endif //GAME_H
