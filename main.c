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

/*******************************************************************************************
* Enums and struct data types
********************************************************************************************/
typedef enum {TITLE = 0, CREDITS, GAMEPLAY, ENDING} GameScreen;

/*******************************************************************************************
* Module Vaiable Defs
********************************************************************************************/
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

const int BOARD_SIDE = 1080;
const int SQUARE_SIDE = BOARD_SIDE/8;
const int CENTER_SQUARE = SQUARE_SIDE / 2;

int Board[8][8] = {0};

int PiecesPlaced = 0;
bool GameOver = false;
int Winner = 0;

bool Player1Turn = true;
Vector2 ClickPosition = {0,0};

/*******************************************************************************************
* Module Function Declarations
********************************************************************************************/
int PlayerPiece() {
    return (Player1Turn) ? 1 : 2;
}

//x: start x
//y: start y
//dx: direction to move towards x
//dy: direction to move towards y
//Return: number of pieces captured
int CheckValid(int x, int y, int dx, int dy) {
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

//x: start x
//y: start y
//Return: If any pieces were flipped
bool FlipPieces(const int x, const int y) {
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
    
    return totalchanged != 0;
}

void EndGame() {
    GameOver = true;
    int p1count = 0;
    int p2count = 0;
    for(int y = 0; y < 8; ++y) {
        for(int x = 0; x < 8; ++x) {
            (Board[y][x] == 1) ? ++p1count  : ++p2count;
        }
    }
    
    if(p1count == p2count) {
        Winner = 0;
    }
    else if(p1count < p2count) {
        Winner = 2;
    }
    else {
        Winner = 1;
    }
}


/*******************************************************************************************
* Main Entry Point
********************************************************************************************/
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    Board[3][4] = 1;
    Board[3][3] = 2;
    Board[4][4] = 2;
    Board[4][3] = 1;
    PiecesPlaced = 4;
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Reversi");
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            ClickPosition = GetMousePosition();
            int x = ClickPosition.x/SQUARE_SIDE;
            int y = ClickPosition.y/SQUARE_SIDE;
            if(FlipPieces(x, y)){
                Board[y][x] = PlayerPiece();
                Player1Turn = !Player1Turn;
                ++PiecesPlaced;
            }
        }
        //For passing
        else if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            Player1Turn = !Player1Turn;
        }
        
        if(PiecesPlaced == 64) {
            EndGame();
        }
        
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        
        ClearBackground((Player1Turn) ? RAYWHITE : BLACK);
        DrawRectangle(0, 0, BOARD_SIDE, BOARD_SIDE, BLACK);
        
        for(int y = 0; y < 8; y++) {
            for(int x = 0; x < 8; x++) {
                int xloc = SQUARE_SIDE*x;
                int yloc = SQUARE_SIDE*y;
                DrawRectangle(xloc, yloc, SQUARE_SIDE - 5, SQUARE_SIDE - 5, DARKGREEN);
                switch(Board[y][x]) {
                    case 1:
                    DrawCircle(xloc + CENTER_SQUARE, yloc + CENTER_SQUARE, CENTER_SQUARE, RAYWHITE);
                    break;
                    case 2:
                    DrawCircle(xloc + CENTER_SQUARE, yloc + CENTER_SQUARE, CENTER_SQUARE, BLACK);
                    break;
                }
            }
        }
        
        if(GameOver) {
            if(Winner == 0) {
                DrawText(TextFormat("Tie Game!"), 10, 40, 20, LIGHTGRAY);
            }
            else {
                DrawText(TextFormat("Player %i Wins!", Winner), 10, 40, 20, LIGHTGRAY);
            }
        }
        
        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    
    return 0;
}

/*******************************************************************************************
* Module Function Defs
********************************************************************************************/