/*******************************************************************************************
*
*   Reversi AI
*
*   This game has been created using raylib 3.7 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Derek Nguyen
*
*   DESCRIPTION
*       Main entry point for the application
* 
********************************************************************************************/

#include "include/raylib.h"
#include "def.h"
#include "game.h"
#include "aiopponent.h"

/*******************************************************************************************
* Module Vaiable Defs
********************************************************************************************/
typedef enum {SELECT = 0, PLAY, OVER} GameState;

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define BGCOLOR (Color){31, 31, 31, 255}

// for draing board
#define DRAW_BOARD_X 460
#define DRAW_BOARD_Y 28
#define DRAW_BOARD_SIDE 1024
#define SQUARE_SIDE 128
#define CENTER_SQUARE 64

// for drawing the square/circle in the sections
#define DRAW_SQUARE 118
#define DRAW_OFFSET 5
#define DRAW_RADIUS 53

#define P1_COLOR BLACK
#define P2_COLOR WHITE
#define PASSIVE_COLOR GRAY

Game* GameInstance = NULL;
GameState State;

bool VsAI = false;
bool P1IsPlayer = false;
bool PlayerTurn = false;

int Winner = 0;

Vector2 ClickPosition = {0,0};

AIOpponent* aiopponent;

/*******************************************************************************************
* Module Function Declarations
********************************************************************************************/
//Draws the board
void DrawBoard();

//Draws the UI
void DrawUI();

//Draws the select
void DrawSelect();

//Resets the game
void Reset();

/*******************************************************************************************
* Main Entry Point
********************************************************************************************/
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Reversi AI");
    SetTargetFPS(30);
    
    //Initialize game
    if (!Game_Init(&GameInstance)) {
        return -1;
    }
    aiopponent = NULL;
    State = SELECT;
    //--------------------------------------------------------------------------------------
    
    // Main game loop
    // Only close on window close button or ESC key
    while (!WindowShouldClose())    
    {
        // Update
        //----------------------------------------------------------------------------------
        switch(State) {
            case SELECT:
            {
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    ClickPosition = GetMousePosition();
                    //Choose if vs Player or vs AI
                    if (ClickPosition.x > SCREEN_WIDTH / 2) {
                        VsAI = true;
                        if (!AI_Init(&aiopponent)) {
                            return -1;
                        }
                        if (ClickPosition.y < SCREEN_HEIGHT / 2) {
                            P1IsPlayer = true;
                            PlayerTurn = true;
                        }
                        else {
                            P1IsPlayer = false;
                            PlayerTurn = false;
                            aiopponent->p1 = true;
                        }
                    }
                    else {
                        VsAI = false;
                    }
                    State = PLAY;
                }
            }
        break;
            case PLAY: {
                //insta end game
                if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)) {
                    State = OVER;
                    Winner = (GameInstance->P1Score > GameInstance->P2Score) ? PLAYER1_PIECE : PLAYER2_PIECE;
                }

                // Player Vs AI
                //--------------------------------------------------------------------------
                if (VsAI) {
                    if (PlayerTurn) {
                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                            ClickPosition = GetMousePosition();
                            int x = (ClickPosition.x - DRAW_BOARD_X) / SQUARE_SIDE;
                            int y = (ClickPosition.y - DRAW_BOARD_Y) / SQUARE_SIDE;

                            //we essentially flip first, and then commit to it if pieces were flipped
                            if (FlipPieces(x, y, P1IsPlayer, GameInstance->Board)) {
                                Game_MoveCommit(GameInstance);
                                PlayerTurn = false;
                                if (Game_CheckOver(GameInstance)) {
                                    State = OVER;
                                    Winner = (GameInstance->P1Score > GameInstance->P2Score) ? PLAYER1_PIECE : PLAYER2_PIECE;
                                }
                            }
                            else {
                                printf("Invalid Move: %c %i \n", x + 'a', y);
                            }
                        }
                        // if passing
                        else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                            Game_MoveCommit(GameInstance);
                            PlayerTurn = false;
                        }
                    }
                    else {
                        if (AI_Choose(aiopponent, GameInstance->Board)) {
                            if (FlipPieces(aiopponent->x, aiopponent->y, aiopponent->p1, GameInstance->Board)) {
                                Game_MoveCommit(GameInstance);
                                PlayerTurn = true;
                                if (Game_CheckOver(GameInstance)) {
                                    State = OVER;
                                    Winner = (GameInstance->P1Score > GameInstance->P2Score) ? PLAYER1_PIECE : PLAYER2_PIECE;
                                }
                            }
                            else {
                                //choose an invalid move
                                printf("Invalid Move: %c %i \n",aiopponent->x + 'a', aiopponent->y);
                            }
                        }
                        else {
                            Game_MoveCommit(GameInstance);
                            PlayerTurn = true;
                        }
                    }
                }
                //--------------------------------------------------------------------------
                
                // Local Player Vs Player
                //--------------------------------------------------------------------------
                else {
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        ClickPosition = GetMousePosition();
                        int x = (ClickPosition.x - DRAW_BOARD_X) / SQUARE_SIDE;
                        int y = (ClickPosition.y - DRAW_BOARD_Y) / SQUARE_SIDE;

                        if (FlipPieces(x, y, GameInstance->P1Turn, GameInstance->Board)) {
                            Game_MoveCommit(GameInstance);
                            if (Game_CheckOver(GameInstance)) {
                                State = OVER;
                                Winner = (GameInstance->P1Score > GameInstance->P2Score) ? PLAYER1_PIECE : PLAYER2_PIECE;
                            }
                        }
                    }
                    else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                        Game_MoveCommit(GameInstance);
                    }
                }
                //--------------------------------------------------------------------------
            }
            break;
            case OVER: {
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    Reset();
                }
            }
            break;
        }
        //----------------------------------------------------------------------------------
        
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        
        ClearBackground(BGCOLOR);
        switch(State) {
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
    if (VsAI) {
        AI_Free(aiopponent);
    }
    Game_Free(GameInstance);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    
    return 0;
}

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
            switch(GameInstance->Board[y][x]) {
                case PLAYER1_PIECE:
                DrawCircle(xloc + CENTER_SQUARE, yloc + CENTER_SQUARE, DRAW_RADIUS, P1_COLOR);
                break;
                case PLAYER2_PIECE:
                DrawCircle(xloc + CENTER_SQUARE, yloc + CENTER_SQUARE, DRAW_RADIUS, P2_COLOR);
                break;
                default:
                break;
            }
            xloc += SQUARE_SIDE;
        }
        yloc += SQUARE_SIDE;
    }
}

void DrawUI() {
    DrawText(TextFormat("%i", GameInstance->P2Score), 1690, 640, 100, (!GameInstance->P1Turn) ? P2_COLOR :PASSIVE_COLOR);
    DrawText(TextFormat("%i", GameInstance->P1Score), 1690, 440, 100, (GameInstance->P1Turn) ? P1_COLOR : PASSIVE_COLOR);
    // Draw AI Values
    if(VsAI) {
        DrawText(TextFormat("Depth Searched: %i", aiopponent->depth), 100, 440, 30, RED);
        DrawText(TextFormat("Eval: %2.2f", aiopponent->eval), 100, 540, 30, ORANGE);
        DrawText(TextFormat("Move: %c%i",(int)aiopponent->x + 'a', aiopponent->y + 1), 100, 640, 30, PURPLE);
    }
    
    if(State == OVER) {
        if(Winner == 0) {
            DrawText(TextFormat("Tie"), 1690, 540, 100, GRAY);
        }
        else {
            DrawText(TextFormat("P%i!", Winner), 1690, 540, 100, GRAY);
        }
    }
}

void DrawSelect() {
    // Draw Local Select
    DrawRectangle(0, 0, 960, SCREEN_HEIGHT, DARKGREEN);
    DrawText(TextFormat("Local"), 380, 440, 100, PASSIVE_COLOR);
    
    // Draw AI Select
    DrawRectangle(960, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, BLACK);
    DrawRectangle(960, 541, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, WHITE);
    DrawText(TextFormat("P1"), 1430, 200, 100, P2_COLOR);
    DrawText(TextFormat("P2"), 1430, 800, 100, P1_COLOR);
}

void Reset() {
    State = SELECT;
    Game_Reset(GameInstance);
}