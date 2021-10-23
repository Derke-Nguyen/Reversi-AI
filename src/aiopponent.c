#include "aiopponent.h"

//Recursive AI move search
int Search(short depth, bool maxer, short tempboard[][BOARD_SIDE]) {
    if(depth == 0) {
        return EvaluateBoardState(maxer, tempboard);
    }
    
    //Generate a list of moves
    MoveList* possiblemoves;
    List_Init(&possiblemoves);
    for(int y = 0; y < BOARD_SIDE; ++y) {
        for(int x = 0; x < BOARD_SIDE; ++x) {
            if(tempboard[y][x] == 0 && HasTargetsAround(x, y, !maxer, tempboard)) {
                int capturepossible = TotalPossibleCapture(x,y, !maxer, tempboard);
                if(capturepossible != 0) {
                    List_Append(possiblemoves, x, y, 1);
                }
            }
        }
    }
    //return if list of moves is 0
    if(possiblemoves->count == 0) {
        return -999;
    }
    int bestEval = -9999999;
    //for each move in list of moves
    MoveListNode* iter = possiblemoves->head;
    short newtempboard[BOARD_SIDE][BOARD_SIDE] = {0};
    while(iter != NULL) {
        //make a board
        for(int y = 0; y < BOARD_SIDE; ++y) {
            for(int x = 0; x < BOARD_SIDE; ++x) {
                newtempboard[y][x] = tempboard[y][x];
            }
        }
        //make move
        short x = iter->x;
        short y = iter->y;
        FlipPieces(x, y, !maxer ,tempboard);
        tempboard[y][x] = (maxer) ? PLAYER2_PIECE : PLAYER1_PIECE;
        
        int evaluation = Search(depth-1, !maxer, newtempboard);
        //set best eval
        if(bestEval < evaluation) {
            bestEval = evaluation;
        }
        
        //undo move
        for(int y = 0; y < BOARD_SIDE; ++y) {
            for(int x = 0; x < BOARD_SIDE; ++x) {
                newtempboard[y][x] = tempboard[y][x];
            }
        }
        iter = iter->next;
    }
    List_Clear(possiblemoves);
    return bestEval;
}

int NumberOfTokensAround(short x, short y, short tempboard[][BOARD_SIDE]) {
    int tokens = 0;
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
            
            if(tempboard[ny][nx] != 0) {
                ++tokens;
            }
        }
    }
    return tokens;
}

float EvaluateBoardState(bool maxer, short tempboard[][BOARD_SIDE]) {
    float score = 0;
    
    //Stability (Measures number of discs that cannot be flipped, High)
    float stbonus = 2;
    
    //Mobility (Measures number of moves currently possible, High early Low late)
    float mobonus = (TurnCount < 30) ? 0.5 : 2;
    score += PossibleMovesCount(maxer, tempboard) * mobonus;
    
    //Corner Grab (Measures if corner capture is possible with its next move, High)
    //Placment (Piece placement score minus piece placement score of opponent)
    float plbonus = 2;
    for(int y = 0; y < BOARD_SIDE; ++y) {
        for(int x = 0; x < BOARD_SIDE; ++x) {
            score += BOARD_VALUES[y][x] * plbonus;
        }
    }
    
    //Frontier Discs (Number of spaces adjacent to opponent pieces minus the the number of spaces adjacent to the current pieces.)
    float frbonus = 1;
    short ppiece = 0;
    short opiece = 0;
    for(int y = 0; y < BOARD_SIDE; ++y) {
        for(int x = 0; x < BOARD_SIDE; ++x) {
            short piece = tempboard[y][x];
            if(piece  == 0) {
                continue;
            }
            else if(piece == PlayerPiece()) {
                ++ppiece;
            }
            else {
                ++opiece;
            }
            
        }
    }
    score += (opiece - ppiece) * frbonus;
    
    //Disc difference (Measures number of discs on the board. Low early Moderate mid High late)
    float ddbonus = (TurnCount < 30) ? 0.5 : 2;
    for(int y = 0; y < BOARD_SIDE; ++y) {
        for(int x = 0; x < BOARD_SIDE; ++x) {
            if(tempboard[y][x] == PlayerPiece()) {
                score += ddbonus;
            }
        }
    }
    
    //Parity (Measures who is expected to make the last move of the game. Low early High late)
    float prbonus = (TurnCount < 30) ? 0.5 : 2;
    for(int y = 0; y < BOARD_SIDE; ++y) {
        for(int x = 0; x < BOARD_SIDE; ++x) {
        }
    }
    
    return score;
}

/*
float GenerateScore(short x, short y, short tempboard[][BOARD_SIDE]) {
    float score = 0;
    //take into account frontier disks vs interior
    score += (8 - NumberOfTokensAround(x, y, tempboard));
    
    //take into account point values for positions
    score += BOARD_VALUES[y][x];
    
    //find stable disks
    return score;
}
*/

Vector2 ChooseComputerMove() {
    DepthSearched = 1;
    
    printf("Appended To Move List: ");
    
    short tempboard[BOARD_SIDE][BOARD_SIDE] = {0};
    
    for(int y = 0; y < BOARD_SIDE; ++y) {
        for(int x = 0; x < BOARD_SIDE; ++x) {
            if(Board[y][x] == 0 && HasTargetsAround(x, y, false, Board)) {
                int capturepossible = TotalPossibleCapture(x,y, false, Board);
                if(capturepossible != 0) {
                    //reset board
                    for(int y = 0; y < BOARD_SIDE; ++y) {
                        for(int x = 0; x < BOARD_SIDE; ++x) {
                            tempboard[y][x] = Board[y][x];
                        }
                    }
                    
                    FlipPieces(x, y, false, tempboard);
                    tempboard[y][x] = PlayerPiece();
                    
                    float score = Search(DepthSearched, true, tempboard);
                    List_Append(AIPossibleMoves, x, y, score);
                    printf("(%i, %i)", x, y);
                }
            }
        }
    }
    printf("\n");
    
    //no possible moves
    Vector2 loc = {-1, -1};
    
    if(AIPossibleMoves->count == 0) {
        return loc;
    }
    
    //options
    //random->(rand() % (AIPossibleMoves->count))
    //highest capture
    int index = List_FindMax(AIPossibleMoves);
    MoveListNode* temp = List_GetMove(AIPossibleMoves, index);
    loc.x = temp->x;
    loc.y = temp->y;
    EvalValue = temp->score;
    List_Clear(AIPossibleMoves);
    return loc;
}