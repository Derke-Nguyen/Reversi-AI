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
*       AI thinking
*
********************************************************************************************/

#include "aiopponent.h"
#include <float.h>

bool AI_Init(AIOpponent** ai){
    *ai = malloc(sizeof(AIOpponent));
    if (*ai == NULL) {
        printf("ERROR: GAME: Failed to initialize AI\n");
        return false;
    }
    if (!List_Init(&((*ai)->moves))) {
        printf("ERROR: GAME: Failed to initialize movelist in AI\n");
        return false;
    }
    (*ai)->depth = 0;
    (*ai)->y = 0;
    (*ai)->x = 0;
    (*ai)->stage = 0;
    (*ai)->p1 = false;
    (*ai)->eval = 0;
    printf("INFO: GAME: Initialized AI\n");
    return true;
}

void AI_Free(AIOpponent* ai) {
    List_Clear(ai->moves);
    free(ai);
}

bool AI_Choose(AIOpponent* ai, int8_t board[][BOARD_SIDE]) {
    ai->depth = (ai->stage < 10) ? 2 : 6;

    int8_t tempboard[BOARD_SIDE][BOARD_SIDE] = { 0 };

    for (int y = 0; y < BOARD_SIDE; ++y) {
        for (int x = 0; x < BOARD_SIDE; ++x) {
            if (board[y][x] == 0 && HasTargetsAround(x, y, ai->p1, board)) {
                //copy board
                CopyBoard(board, tempboard);
                if (FlipPieces(x, y, ai->p1, tempboard)) {
                    float score = Search(ai->depth, -FLT_MAX, FLT_MAX, ai->stage, false, ai->p1, tempboard);

                    List_Append(ai->moves, x, y, score);
                }
            }
        }
    }

    if (ai->moves->count == 0) {
        return false;
    }

    //highest capture
    MoveListNode* max = List_GetMax(ai->moves);
    ai->x = max->x;
    ai->y = max->y;
    ai->eval = max->score;
    ai->stage += 1;
    List_Clear(ai->moves);
    return true;
}

float BOARD_VALUES[BOARD_SIDE][BOARD_SIDE] = {
    {16.16, -3.03,  0.99,  0.43,  0.43,  0.99, -3.03, 16.16},
    {-4.12, -1.81, -0.08, -0.27, -0.27, -0.08, -1.81, -4.12},
    {1.33 , -0.04,  0.51,  0.07,  0.07,  0.51, -0.04, 1.33 },
    {0.6  , -0.18, -0.04, -0.01, -0.01, -0.04, -0.18, 0.6  },
    {0.6  , -0.18, -0.04, -0.01, -0.01, -0.04, -0.18, 0.6  },
    {1.33 , -0.04,  0.51,  0.07,  0.07,  0.51, -0.04, 1.33 },
    {-4.12, -1.81, -0.08, -0.27, -0.27, -0.08, -1.81, -4.12},
    {16.16, -3.03,  0.99,  0.43,  0.43,  0.99, -3.03, 16.16}
};
float EvaluateBoardState(bool player1, int8_t stage, int8_t tempboard[][BOARD_SIDE]) {
    float score = 0;
    //1 low
    //5 middle
    //10 high

    //Stability (Measures number of discs that cannot be flipped, High)
    float stbonus = 10;
    int8_t stablecount = 0;
    for (int y = 0; y < BOARD_SIDE; ++y) {
        for (int x = 0; x < BOARD_SIDE; ++x) {
            if (IsStable(x, y, tempboard) && ((tempboard[y][x] == PLAYER1_PIECE && player1) || (tempboard[y][x] == PLAYER2_PIECE && !player1))) {
                ++stablecount;
            }
        }
    }
    score += (stablecount)*stbonus;

    //Mobility (Measures number of moves currently possible, High early Low late)
    float mobonus = (stage < 10) ? 1 : 10;
    score += PossibleMovesCount(player1, tempboard) * mobonus;


    //Corner Grab (Measures if corner capture is possible with its next move, High)
    float cgbonus = 10;
    if (tempboard[0][0] == (int8_t)((player1) ? PLAYER1_PIECE : PLAYER2_PIECE)) {
        score += cgbonus;
    }
    if (tempboard[0][BOARD_SIDE - 1] == (int8_t)((player1) ? PLAYER1_PIECE : PLAYER2_PIECE)) {
        score += cgbonus;
    }
    if (tempboard[BOARD_SIDE - 1][0] == (int8_t)((player1) ? PLAYER1_PIECE : PLAYER2_PIECE)) {
        score += cgbonus;
    }
    if (tempboard[BOARD_SIDE - 1][BOARD_SIDE - 1] == (int8_t)((player1) ? PLAYER1_PIECE : PLAYER2_PIECE)) {
        score += cgbonus;
    }

    //Placment (Piece placement score minus piece placement score of opponent)
    float plbonus = 10;
    for (int y = 0; y < BOARD_SIDE; ++y) {
        for (int x = 0; x < BOARD_SIDE; ++x) {
            score += BOARD_VALUES[y][x] * plbonus;
        }
    }

    //Frontier Discs (Number of spaces adjacent to opponent pieces minus the the number of spaces adjacent to the current pieces.)
    float frbonus = 5;
    int8_t p1piece = 0;
    int8_t p2piece = 0;
    for (int y = 0; y < BOARD_SIDE; ++y) {
        for (int x = 0; x < BOARD_SIDE; ++x) {
            int8_t piece = tempboard[y][x];
            if (piece == 0) {
                continue;
            }
            else if (IsFrontier(x, y, tempboard))
            {
                if (piece == PLAYER1_PIECE) {
                    ++p1piece;
                }
                else {
                    ++p2piece;
                }
            }
        }
    }
    score += ((player1) ? (p1piece - p2piece) : (p2piece - p1piece)) * frbonus;

    //Disc difference (Measures number of discs on the board. Low early Moderate mid High late)
    float ddbonus = (stage < 10) ? 10 : 1;
    p1piece = 0;
    p2piece = 0;
    for (int y = 0; y < BOARD_SIDE; ++y) {
        for (int x = 0; x < BOARD_SIDE; ++x) {
            int8_t piece = tempboard[y][x];
            if (piece == 0) {
                continue;
            }
            else if (piece == PLAYER1_PIECE) {
                ++p1piece;
            }
            else {
                ++p2piece;
            }

        }
    }
    score += ((player1) ? (p2piece - p1piece) : (p1piece - p2piece)) * ddbonus;

    //TODO: Parity (Measures who is expected to make the last move of the game. Low early High late)

    return score;
}

float Search(int8_t depth, float alpha, float beta, int8_t stage, bool maxer, bool player1, int8_t tempboard[][BOARD_SIDE]) {
    if(depth == 0) {
        return EvaluateBoardState(player1 == maxer, stage, tempboard);
    }
    
    //Generate a list of moves
    MoveList* possiblemoves;
    List_Init(&possiblemoves);
    for(int y = 0; y < BOARD_SIDE; ++y) {
        for(int x = 0; x < BOARD_SIDE; ++x) {
            if(tempboard[y][x] == 0 && HasTargetsAround(x, y, player1, tempboard)) {
                int capturepossible = TotalPossibleCapture(x,y, player1, tempboard);
                if(capturepossible != 0) {
                    List_Append(possiblemoves, x, y, -FLT_MAX);
                }
            }
        }
    }

    //return if list of moves is 0, pass
    if(possiblemoves->count == 0) {
        return EvaluateBoardState(player1 == maxer, stage, tempboard);
    }

    //for each move in list of moves
    MoveListNode* iter = possiblemoves->head;
    int8_t newtempboard[BOARD_SIDE][BOARD_SIDE] = {0};
    while(iter != NULL) {
        //make a board
        for(int y = 0; y < BOARD_SIDE; ++y) {
            for(int x = 0; x < BOARD_SIDE; ++x) {
                newtempboard[y][x] = tempboard[y][x];
            }
        }
        //make move
        int8_t x = iter->x;
        int8_t y = iter->y;
        FlipPieces(x, y, player1 ,tempboard);
        
        float evaluation = -Search(depth-1, -beta, -alpha, stage + 1, !maxer, !player1, newtempboard);
        printf("Depth: %i, Eval: %0.2f, Move: %i,%i\n", depth, evaluation, x, y);
        //undo move
        for(int y = 0; y < BOARD_SIDE; ++y) {
            for(int x = 0; x < BOARD_SIDE; ++x) {
                newtempboard[y][x] = tempboard[y][x];
            }
        }

        //snip because alpha beta
        if (evaluation >= beta) {
            return beta;
        }
        
        alpha = MAX(alpha, evaluation);

        iter = iter->next;
    }
    List_Free(possiblemoves);
    return alpha;
}

bool IsStable(int8_t x, int8_t y, int8_t tempboard[][BOARD_SIDE]) {
    //horizontal
    if (x + 1 != BOARD_SIDE && x - 1 > -1) {
        if (tempboard[y][x + 1] != tempboard[y][x - 1]) {
            return false;
        }
    }

    //vertical
    if (y + 1 != BOARD_SIDE && y - 1 > -1) {
        if (tempboard[y + 1][x] != tempboard[y - 1][x]) {
            return false;
        }
    }

    //back diag
    if ((x - 1 > -1 && y - 1 > -1) && (y + 1 != BOARD_SIDE && x + 1 != BOARD_SIDE)) {
        if (tempboard[y - 1][x - 1] != tempboard[y + 1][x + 1]) {
            return false;
        }
    }

    //forward diag
    if ((x - 1 > -1 && y + 1 != BOARD_SIDE) && (y - 1 > -1 && x + 1 != BOARD_SIDE)) {
        if (tempboard[y + 1][x - 1] != tempboard[y - 1][x + 1]) {
            return false;
        }
    }
    return true;
}

int NumberOfTokensAround(int8_t x, int8_t y, int8_t tempboard[][BOARD_SIDE]) {
    int tokens = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        int8_t ny = y + dy;
        if (ny < 0 || ny >= 8) {
            continue;
        }
        for (int8_t dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) {
                continue;
            }

            int8_t nx = x + dx;
            if (nx < 0 || nx >= 8) {
                continue;
            }

            if (tempboard[ny][nx] != 0) {
                ++tokens;
            }
        }
    }
    return tokens;
}

int PossibleMovesCount(bool player1, int8_t tempboard[][BOARD_SIDE]) {
    int result = 0;
    for (int y = 0; y < BOARD_SIDE; ++y) {
        for (int x = 0; x < BOARD_SIDE; ++x) {
            if (tempboard[y][x] == 0 && HasTargetsAround(x, y, player1, tempboard)) {
                int capturepossible = TotalPossibleCapture(x, y, player1, tempboard);
                if (capturepossible != 0) {
                    ++result;
                }
            }
        }
    }
    return result;
}

bool IsFrontier(int8_t x, int8_t y, int8_t tempboard[][BOARD_SIDE]) {
    for (int dy = -1; dy <= 1; ++dy) {
        short ny = y + dy;
        if (ny < 0 || ny >= 8) {
            continue;
        }
        for (short dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) {
                continue;
            }

            short nx = x + dx;
            if (nx < 0 || nx >= 8) {
                continue;
            }

            if (tempboard[ny][nx] == 0) {
                true;
            }
        }
    }


    return false;
}

int8_t TotalPossibleCapture(int8_t x, int8_t y, bool player1, int8_t board[][BOARD_SIDE]) {
    if (board[y][x] != 0) {
        return 0;
    }

    int totalcapture = 0;

    //check left
    if (x - 1 > -1)
        totalcapture += PiecesInDirection(x - 1, y, -1, 0, player1, board);

    //check right
    if (x + 1 < 8)
        totalcapture += PiecesInDirection(x + 1, y, 1, 0, player1, board);

    //check up left
    if (x - 1 > -1 && y - 1 > -1)
        totalcapture += PiecesInDirection(x - 1, y - 1, -1, -1, player1, board);

    //check up
    if (y - 1 > -1)
        totalcapture += PiecesInDirection(x, y - 1, 0, -1, player1, board);

    //check up right
    if (x + 1 < 8 && y - 1 > -1)
        totalcapture += PiecesInDirection(x + 1, y - 1, 1, -1, player1, board);

    //check down left
    if (x - 1 > -1 && y + 1 < 8)
        totalcapture += PiecesInDirection(x - 1, y + 1, -1, 1, player1, board);

    //check down
    if (y + 1 < 8)
        totalcapture += PiecesInDirection(x, y + 1, 0, 1, player1, board);

    //check down right
    if (x + 1 < 8 && y + 1 < 8)
        totalcapture += PiecesInDirection(x + 1, y + 1, 1, 1, player1, board);

    return totalcapture;
}

bool HasTargetsAround(int8_t x, int8_t y, bool player1, int8_t board[][BOARD_SIDE]) {
    for (int8_t dy = -1; dy <= 1; ++dy) {
        int8_t ny = y + dy;
        if (ny < 0 || ny >= 8) {
            continue;
        }
        for (int8_t dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) {
                continue;
            }

            int8_t nx = x + dx;
            if (nx < 0 || nx >= 8) {
                continue;
            }

            if (board[ny][nx] != 0 && board[ny][nx] != ((int8_t)(player1) ? PLAYER1_PIECE : PLAYER2_PIECE)) {
                return true;
            }
        }
    }
    return false;
}