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
#include "game.h"
#include "movelist.h"
#include "def.h"
#include <stdlib.h>
#include <stdio.h>

/*******************************************************************************************
* Enums and struct data types
********************************************************************************************/
typedef enum {SELECT = 0, PLAY, OVER} GameState;

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

Game* Game;

short Board[BOARD_SIDE][BOARD_SIDE] = {0};

bool P1IsPlayer = true;

bool GameOver = false;
int Winner = 0;

Vector2 ClickPosition = {0,0};
int TurnCount = 0;

/*******************************************************************************************
* Module Function Declarations
********************************************************************************************/
//Draws the board
void DrawBoard();

//Draws the UI
void DrawUI();

//Draws the select
void DrawSelect();

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
                        int totalchanged = FlipPieces(x, y, true, Board);
                        if(totalchanged != 0){
                            Board[y][x] = PlayerPiece();
                            if(P1Turn) {
                                P1Score += totalchanged;
                                P2Score -= totalchanged;
                            }
                            else {
                                P2Score += totalchanged;
                                P1Score -= totalchanged;
                            }
                            (P1Turn) ? ++P1Score : ++P2Score;
                            P1Turn = !P1Turn;
                            ++TurnCount;
                        }
                    }
                    //For passing
                    else if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                        P1Turn = !P1Turn;
                        ++TurnCount;
                    }
                }
                //AI turn
                else {
                    ClickPosition = ChooseComputerMove();
                    int x = ClickPosition.x;
                    int y = ClickPosition.y;
                    AIMoveChosen.x = x;
                    AIMoveChosen.y = y;
                    
                    //pass if no possible moves
                    if(x == -1) {
                        P1Turn = !P1Turn;
                        ++TurnCount;
                    }
                    
                    int totalchanged = FlipPieces(x, y, false, Board);
                    if(totalchanged != 0){
                        Board[y][x] = PlayerPiece();
                        if(P1Turn) {
                            P1Score += totalchanged;
                            P2Score -= totalchanged;
                        }
                        else {
                            P2Score += totalchanged;
                            P1Score -= totalchanged;
                        }
                        (P1Turn) ? ++P1Score : ++P2Score;
                        P1Turn = !P1Turn;
                        ++TurnCount;
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
                DrawSelect();
            }
            break;
            case PLAY:
            case OVER:
            {
                DrawBoard();
                DrawUI();
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

void DrawUI() {
    DrawText(TextFormat("%i", P2Score), 1690, 440, 100, (!P1Turn) ? P2_COLOR :PASSIVE_COLOR);
    DrawText(TextFormat("%i", P1Score), 1690, 640, 100, (P1Turn) ? P1_COLOR : PASSIVE_COLOR);
    //Draw AI Values
    DrawText(TextFormat("Depth Searched: %i", DepthSearched), 100, 440, 30, RED);
    DrawText(TextFormat("Eval: %2.2f", EvalValue), 100, 540, 30, ORANGE);
    DrawText(TextFormat("Move: %c%.0f",(int)AIMoveChosen.x + 'a', AIMoveChosen.y + 1), 100, 640, 30, PURPLE);
    if(GameOver) {
        if(Winner == 0) {
            DrawText(TextFormat("Tie"), 1690, 540, 100, GRAY);
        }
        else {
            DrawText(TextFormat("P%i!", Winner), 1690, 540, 100, GRAY);
        }
    }
}

void DrawSelect() {
    DrawRectangle(0, 0, 960, SCREEN_HEIGHT, BLACK);
    DrawRectangle(961, 0, 960, SCREEN_HEIGHT, WHITE);
    DrawText(TextFormat("P1"), 450, 440, 100, P2_COLOR);
    DrawText(TextFormat("P2"), 1430, 440, 100, P1_COLOR);
}
