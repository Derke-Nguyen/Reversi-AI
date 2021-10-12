/*******************************************************************************************
*
*   Reversi AI
*
*   This game has been created using raylib 3.5 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Derek Nguyen
*
********************************************************************************************/

#include "include/raylib.h"
#include "list.h"
#include <stdlib.h>
#include <stdio.h>

/*******************************************************************************************
* Enums and struct data types
********************************************************************************************/
typedef enum {SELECT = 0, PLAY, OVER} GameState;

struct Piece {
    bool empty;
    bool p1;
};

/*******************************************************************************************
* Enums and Struct Function Defs
********************************************************************************************/

/*******************************************************************************************
* Module Vaiable Defs
********************************************************************************************/
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define BGCOLOR (Color){31, 31, 31, 255}

#define DRAW_BOARD_X 460
#define DRAW_BOARD_Y 28
#define DRAW_BOARD_SIDE 1024
#define SQUARE_SIDE 128
#define CENTER_SQUARE 64

//for drawing the square/circle in the sections
#define DRAW_SQUARE 118
#define DRAW_OFFSET 5
#define DRAW_RADIUS 53

//UI related things
#define P1_COLOR BLACK
#define P2_COLOR WHITE
#define PASSIVE_COLOR GRAY

#define BOARD_SIDE 8

short Board[BOARD_SIDE][BOARD_SIDE] = {0};
bool P1IsPlayer = true;

bool GameOver = false;
int Winner = 0;

int P1Score = 0;
int P2Score = 0;

bool P1Turn = true;
Vector2 ClickPosition = {0,0};

//AI Related
MoveList* AIPossibleMoves;

/*******************************************************************************************
* Module Function Declarations
********************************************************************************************/
// Gameplay Functions
//-----------------------------------------------------------------------------------------
//Determines which color piece to place based on player turn
//@Return: the piece to place
short PlayerPiece();

//Checks if it's the player's turn
//@Return true if player turn matches player
bool IsPlayerTurn();

//Checks if there are any pieces around it that can be flipped
//@Return true if there is an opponent token
bool HasTargetsAround(short x, short y);

//x: start x
//y: start y
//dx: direction to move towards x
//dy: direction to move towards y
//Return: number of pieces captured
int CheckValid(short x, short y, short dx, short dy);

//x: start x
//y: start y
//Return: number of pieces to capture in all directions
int TotalPossibleCapture(short x, short y) {
    if(Board[y][x] != 0) {
        return 0;
    }
    
    int totalcapture = 0;
    
    //check left
    if(x-1 > -1)
        totalcapture += CheckValid(x - 1, y, -1, 0);
    
    //check right
    if(x+1 < 8)
        totalcapture += CheckValid(x + 1, y, 1, 0);
    
    //check up left
    if(x-1 > -1 && y-1 > -1)
        totalcapture += CheckValid(x - 1, y - 1, -1, -1);
    
    //check up
    if(y-1 > -1)
        totalcapture += CheckValid(x , y - 1, 0, -1);
    
    //check up right
    if(x+1 < 8 && y-1 > -1)
        totalcapture += CheckValid(x + 1, y - 1, 1, -1);
    
    //check down left
    if(x-1 > -1 && y+1 < 8)
        totalcapture += CheckValid(x - 1, y + 1, -1, 1);
    
    //check down
    if(y+1 < 8)
        totalcapture += CheckValid(x, y + 1, 0, 1);
    
    //check down right
    if(x+1 < 8 && y+1 < 8)
        totalcapture += CheckValid(x + 1, y + 1, 1, 1);
    
    return totalcapture;
}

//x: start x
//y: start y
//Return: If any pieces were flipped
bool FlipPieces(const short x, const short y);

//Init the board
void InitBoard();

//Draws the board
void DrawBoard();

//Resets the game
void ResetGame();

//Determines the winner and ends the game
void EndGame();

// AI Functions
//-----------------------------------------------------------------------------------------
//Selects a move for AI to choose from
void AIGenerateMoves() {
    for(int y = 0; y < BOARD_SIDE; ++y) {
        for(int x = 0; x < BOARD_SIDE; ++x) {
            if(Board[y][x] != PlayerPiece()) {
                //check pieces around for empty spot and then run a checker
                
            }
        }
    }
}

Vector2 ChooseComputerMove() {
    for(int y = 0; y < BOARD_SIDE; ++y) {
        for(int x = 0; x < BOARD_SIDE; ++x) {
            if(Board[y][x] == 0 && HasTargetsAround(x, y)) {
                if(TotalPossibleCapture(x,y) != 0) {
                    List_Append(AIPossibleMoves, x, y);
                    printf("(%i, %i)", x, y);
                }
            }
        }
    }
    
    
    //no possible moves
    Vector2 loc = {-1, -1};
    
    if(AIPossibleMoves->count == 0) {
        return loc;
    }
    
    //pick a random number
    int index = (rand() % (AIPossibleMoves->count));
    MoveListNode* temp = List_GetMove(AIPossibleMoves, index);
    loc.x = temp->x;
    loc.y = temp->y;
    //List_Clear(AIPossibleMoves);
    return loc;
}

/*******************************************************************************************
* Main Entry Point
********************************************************************************************/
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Reversi AI");
    SetTargetFPS(30);               // Set our game to run at 30 frames-per-second
    GameState state = SELECT;
    InitBoard();
    AIPossibleMoves = NULL;
    List_Init(&AIPossibleMoves);
    //--------------------------------------------------------------------------------------
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        switch(state) {
            case SELECT:
            {
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    ClickPosition = GetMousePosition();
                    if(ClickPosition.x > SCREEN_WIDTH/2) {
                        P1IsPlayer = false;
                    }
                    state = PLAY;
                }
                
            }
            break;
            case PLAY:{
                //TODO: If player is not p1
                if(P1Turn) {
                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        ClickPosition = GetMousePosition();
                        int x = (ClickPosition.x - DRAW_BOARD_X)/SQUARE_SIDE;
                        int y = (ClickPosition.y - DRAW_BOARD_Y)/SQUARE_SIDE;
                        if(FlipPieces(x, y)){
                            Board[y][x] = PlayerPiece();
                            (P1Turn) ? ++P1Score : ++P2Score;
                            P1Turn = !P1Turn;
                        }
                    }
                    //For passing
                    else if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                        P1Turn = !P1Turn;
                    }
                }
                //AI turn
                else {
                    ClickPosition = ChooseComputerMove();
                    int x = ClickPosition.x;
                    int y = ClickPosition.y;
                    
                    //pass if no possible moves
                    if(x == -1) {
                        P1Turn = !P1Turn;
                    }
                    
                    if(FlipPieces(x, y)){
                        Board[y][x] = PlayerPiece();
                        (P1Turn) ? ++P1Score : ++P2Score;
                        P1Turn = !P1Turn;
                    }
                }
                
                if(P1Score + P2Score == 64 || P1Score == 0 || P2Score == 0) {
                    EndGame();
                    state = OVER;
                }
            }
            break;
            case OVER: {
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    ResetGame();
                }
            }
            break;
        }
        
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        
        ClearBackground(BGCOLOR);
        switch(state) {
            case SELECT:
            {
                DrawRectangle(0, 0, 960, SCREEN_HEIGHT, BLACK);
                DrawRectangle(961, 0, 960, SCREEN_HEIGHT, WHITE);
                DrawText(TextFormat("P1"), 450, 440, 100, P2_COLOR);
                DrawText(TextFormat("P2"), 1430, 440, 100, P1_COLOR);
            }
            break;
            case PLAY:
            case OVER:
            {
                DrawBoard();
                DrawText(TextFormat("%i", P2Score), 1690, 440, 100, (!P1Turn) ? P2_COLOR :PASSIVE_COLOR);
                DrawText(TextFormat("%i", P1Score), 1690, 640, 100, (P1Turn) ? P1_COLOR : PASSIVE_COLOR);
                if(GameOver) {
                    if(Winner == 0) {
                        DrawText(TextFormat("Tie"), 1690, 540, 100, GRAY);
                    }
                    else {
                        DrawText(TextFormat("P%i!", Winner), 1690, 540, 100, GRAY);
                    }
                }
            }
            break;
            default:
            break;
        }
        
        
        
        
        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    
    // De-Initialization
    //--------------------------------------------------------------------------------------
    free(AIPossibleMoves);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    
    return 0;
}

/*******************************************************************************************
* Enums and Struct Function Defs
********************************************************************************************/

/*******************************************************************************************
* Module Function Defs
********************************************************************************************/
short PlayerPiece() {
    return (P1Turn) ? 1 : 2;
}

bool IsPlayerTurn() {
    return (P1Turn == P1IsPlayer);
}

bool HasTargetsAround(short x, short y) {
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
            
            if(Board[ny][nx] != 0 && Board[ny][nx] != PlayerPiece()) {
                return true;
            }
        }
    }
    return false;
}

int CheckValid(short x, short y, short dx, short dy) {
    int captured = 0;
    while((x > -1 && x < 8) && (y > -1 && y < 8)) {
        if(Board[y][x] == 0) {
            return 0;
        }
        
        if(Board[y][x] == PlayerPiece()){
            return captured;
        }
        ++captured;
        x += dx;
        y += dy;
    }
    
    return 0;
}

bool FlipPieces(const short x, const short y) {
    if(Board[y][x] != 0) {
        return false;
    }
    
    int totalchanged = 0;
    int captured = 0;
    
    //check left
    captured = CheckValid(x - 1, y, -1, 0);
    totalchanged += captured;
    for(int i = 0; i < captured; ++i) {
        Board[y][x - 1 - i] = PlayerPiece();
    }
    
    //check right
    captured = CheckValid(x + 1, y, 1, 0);
    totalchanged += captured;
    for(int i = 0; i < captured; ++i) {
        Board[y][x + 1 + i] = PlayerPiece();
    }
    
    //check up left
    captured = CheckValid(x - 1, y - 1, -1, -1);
    totalchanged += captured;
    for(int i = 0; i < captured; ++i) {
        Board[y - 1 - i][x - 1 - i] = PlayerPiece();
    }
    
    //check up
    captured = CheckValid(x , y - 1, 0, -1);
    totalchanged += captured;
    for(int i = 0; i < captured; ++i) {
        Board[y - 1 - i][x] = PlayerPiece();
    }
    
    //check up right
    captured = CheckValid(x + 1, y - 1, 1, -1);
    totalchanged += captured;
    for(int i = 0; i < captured; ++i) {
        Board[y - 1 - i][x + 1 + i] = PlayerPiece();
    }
    
    //check down left
    captured = CheckValid(x - 1, y + 1, -1, 1);
    totalchanged += captured;
    for(int i = 0; i < captured; ++i) {
        Board[y + 1 + i][x - 1 - i] = PlayerPiece();
    }
    
    //check down
    captured = CheckValid(x, y + 1, 0, 1);
    totalchanged += captured;
    for(int i = 0; i < captured; ++i) {
        Board[y + 1 + i][x] = PlayerPiece();
    }
    
    //check down right
    captured = CheckValid(x + 1, y + 1, 1, 1);
    totalchanged += captured;
    for(int i = 0; i < captured; ++i) {
        Board[y + 1 + i][x + 1 + i] = PlayerPiece();
    }
    
    if(P1Turn) {
        P1Score += totalchanged;
        P2Score -= totalchanged;
    }
    else {
        P2Score += totalchanged;
        P1Score -= totalchanged;
    }
    return totalchanged != 0;
}

void EndGame() {
    GameOver = true;
    
    if(P1Score == P2Score) {
        Winner = 0;
    }
    else if(P1Score < P2Score) {
        Winner = 2;
    }
    else {
        Winner = 1;
    }
}

void InitBoard() {
    Board[3][4] = 1;
    Board[3][3] = 2;
    Board[4][4] = 2;
    Board[4][3] = 1;
    P1Score = 2;
    P2Score = 2;
    P1Turn = true;
}

void DrawBoard() {
    DrawRectangle(DRAW_BOARD_X, DRAW_BOARD_Y, BOARD_SIDE, BOARD_SIDE, BLACK);
    
    int xloc = DRAW_BOARD_X;
    int yloc = DRAW_BOARD_Y;
    
    for(int y = 0; y < BOARD_SIDE; ++y) {
        xloc = DRAW_BOARD_X;
        for(int x = 0; x < BOARD_SIDE; ++x) {
            DrawRectangle(xloc + DRAW_OFFSET, yloc + DRAW_OFFSET, DRAW_SQUARE, DRAW_SQUARE, DARKGREEN);
            switch(Board[y][x]) {
                case 1:
                DrawCircle(xloc + CENTER_SQUARE, yloc + CENTER_SQUARE, DRAW_RADIUS, P1_COLOR);
                break;
                case 2:
                DrawCircle(xloc + CENTER_SQUARE, yloc + CENTER_SQUARE, DRAW_RADIUS, P2_COLOR);
                break;
            }
            
            xloc += SQUARE_SIDE;
        }
        yloc += SQUARE_SIDE;
    }
}

void ResetGame() {
    
    for(int y = 0; y < BOARD_SIDE; ++y) {
        for(int x = 0; x < BOARD_SIDE; ++x) {
            Board[y][x] = 0;
        }
    }
    InitBoard();
}
