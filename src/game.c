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
*       All possible ways to interact with the game
*
********************************************************************************************/

#include "game.h"

bool Game_Init(Game** game) {
    *game = malloc(sizeof(Game));
    if (*game == NULL) {
        printf("ERROR: GAME: Failed to initialize game\n");
        return false;
    }
    (*game)->P1Turn = true;
    (*game)->P1Score = 2;
    (*game)->P2Score = 2;
    InitBoard((*game)->Board);
    return true;
}

void Game_Reset(Game* game) {
    for (int8_t y = 0; y < BOARD_SIDE; ++y) {
        for (int8_t x = 0; x < BOARD_SIDE; ++x) {
            game->Board[y][x] = EMPTY_PIECE;
        }
    }
    game->P1Turn = true;
    game->P1Score = 2;
    game->P2Score = 2;
    InitBoard(game->Board);
}

void Game_Free(Game* game) {
    free(game);
}

bool Game_CheckOver(Game* game) {
    if (game->P1Score + game->P2Score == 64 || game->P1Score == 0 || game->P2Score == 0) {
        return true;
    }
    return false;
}

void Game_MoveCommit(Game* game) {
    int8_t p1score = 0;
    int8_t p2score = 0;
    for (int8_t y = 0; y < BOARD_SIDE; ++y) {
        for (int8_t x = 0; x < BOARD_SIDE; ++x) {
            if (game->Board[y][x] == PLAYER1_PIECE) {
                ++p1score;
            }
            else if (game->Board[y][x] == PLAYER2_PIECE) {
                ++p2score;
            }
        }
    }

    game->P1Score = p1score;
    game->P2Score = p2score;
    game->P1Turn = !(game->P1Turn);
}

void InitBoard(int8_t board[][BOARD_SIDE]) {
    board[3][4] = 1;
    board[3][3] = 2;
    board[4][4] = 2;
    board[4][3] = 1;
}

void CopyBoard(int8_t board[][BOARD_SIDE], int8_t newboard[][BOARD_SIDE]) {
    for (int8_t y = 0; y < BOARD_SIDE; ++y) {
        for (int8_t x = 0; x < BOARD_SIDE; ++x) {
            newboard[y][x] = board[y][x];
        }
    }
}

int8_t PiecesInDirection(int8_t x, int8_t y, int8_t dx, int8_t dy, bool player1, int8_t board[][BOARD_SIDE]) {
    int8_t captured = 0;
    int8_t currpiece = (player1) ? PLAYER1_PIECE : PLAYER2_PIECE;
    while((x > -1 && x < 8) && (y > -1 && y < 8)) {
        if(board[y][x] == EMPTY_PIECE) {
            return 0;
        }
        
        if(board[y][x] == currpiece){
            return captured;
        }
        ++captured;
        x += dx;
        y += dy;
    }
    
    return 0;
}

bool FlipPieces(const int8_t x, const int8_t y, bool player1, int8_t board[][BOARD_SIDE]) {
    if(board[y][x] != 0) {
        return false;
    }
    
    int8_t totalchanged = 0;
    int8_t captured = 0;
    int8_t currpiece = (player1) ? PLAYER1_PIECE : PLAYER2_PIECE;
    //check left
    captured = PiecesInDirection(x - 1, y, -1, 0, player1, board);
    totalchanged += captured;
    for(int8_t i = 0; i < captured; ++i) {
        board[y][x - 1 - i] = currpiece;
    }
    //check right
    captured = PiecesInDirection(x + 1, y, 1, 0, player1, board);
    totalchanged += captured;
    for(int8_t i = 0; i < captured; ++i) {
        board[y][x + 1 + i] = currpiece;
    }
    //check up left
    captured = PiecesInDirection(x - 1, y - 1, -1, -1, player1, board);
    totalchanged += captured;
    for(int8_t i = 0; i < captured; ++i) {
        board[y - 1 - i][x - 1 - i] = currpiece;
    }
    //check up
    captured = PiecesInDirection(x , y - 1, 0, -1, player1, board);
    totalchanged += captured;
    for(int8_t i = 0; i < captured; ++i) {
        board[y - 1 - i][x] = currpiece;
    }
    //check up right
    captured = PiecesInDirection(x + 1, y - 1, 1, -1, player1, board);
    totalchanged += captured;
    for(int8_t i = 0; i < captured; ++i) {
        board[y - 1 - i][x + 1 + i] = currpiece;
    }
    //check down left
    captured = PiecesInDirection(x - 1, y + 1, -1, 1, player1, board);
    totalchanged += captured;
    for(int8_t i = 0; i < captured; ++i) {
        board[y + 1 + i][x - 1 - i] = currpiece;
    }
    //check down
    captured = PiecesInDirection(x, y + 1, 0, 1, player1, board);
    totalchanged += captured;
    for(int8_t i = 0; i < captured; ++i) {
        board[y + 1 + i][x] = currpiece;
    }
    //check down right
    captured = PiecesInDirection(x + 1, y + 1, 1, 1, player1, board);
    totalchanged += captured;
    for(int8_t i = 0; i < captured; ++i) {
        board[y + 1 + i][x + 1 + i] = currpiece;
    }
    if (totalchanged != 0)
    {
        board[y][x] = currpiece;
        totalchanged++;
        return true;
    }

    return false;
}