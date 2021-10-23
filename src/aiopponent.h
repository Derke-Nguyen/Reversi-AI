/* date = October 22nd 2021 6:27 pm */

#ifndef AIOPPONENT_H
#define AIOPPONENT_H

#include "def.h"
#include "movelist.h"

#endif //AIOPPONENT_H

//AI Entity
typedef struct AIOpponent {
    MoveList* AIPossibleMoves;
    float EvalValue = 0;
    short DepthSearched = 0;
    Vector2 AIMoveChosen = {0,0};
}AIOpponent;

// AI Functions
//-----------------------------------------------------------------------------------------
//Creates a tree of possible board moves and results
//depth: Recursive depth count
//tempboard: a temp board state
//@Return: the evaluation score
int Search(short depth, bool maxer, short tempboard[][BOARD_SIDE]);

// Gets the number of tokens surrounding a location
//x: x location 
//y: y location
//@Return: number of tokens surrounding a piece
int NumberOfTokensAround(short x, short y, short tempboard[][BOARD_SIDE]);

// Generates score of the board state
//@Return: 
float EvaluateBoardState(bool maxer, short tempboard[][BOARD_SIDE]);

//Genereates a score based on a location
//x: x location
//y: y location
//@Return: the score of a certain move
//float GenerateScore(short x, short y, short tempboard[][BOARD_SIDE])

//Chooses a move generated from minimax
//@Return: The location of the move selected
Vector2 ChooseComputerMove();


int PossibleMovesCount(bool player1, short tempboard[][BOARD_SIDE]) {
    int result = 0;
    for(int y = 0; y < BOARD_SIDE; ++y) {
        for(int x = 0; x < BOARD_SIDE; ++x) {
            if(Board[y][x] == 0 && HasTargetsAround(x, y, player1, Board)) {
                int capturepossible = TotalPossibleCapture(x, y, player1, Board);
                if(capturepossible != 0) {
                    ++result;
                }
            }
        }
    }
    return result;
}

bool IsFrontier(short x, short y, short tempboard[][BOARD_SIDE]) {
    //int tokens = 0;
    //short currPiece = tempboard[y][x];
    for(int dy = -1; dy <= 1; ++dy) {
        short ny = y + dy;
        if(ny < 0 || ny >= 8) {
            continue;
        }
        for(short dx = -1; dx <= 1; ++dx) {
            if(dx == 0 && dy == 0){
                continue;
            }
            
            short nx = x + dx;
            if(nx < 0 || nx >= 8) {
                continue;
            }
            
            if(tempboard[ny][nx] == 0) {
                true;
            }
        }
    }
    
    
    return false;
}
