short PlayerPiece() {
    return (P1Turn) ? 1 : 2;
}

short OpponentPiece() {
    return (P1Turn) ? 2 : 1;
}

bool IsPlayerTurn() {
    return (P1Turn == P1IsPlayer);
}

bool HasTargetsAround(short x, short y, bool player1, short board[][BOARD_SIDE]) {
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
            
            if(board[ny][nx] != 0 && board[ny][nx] != ((short)(player1) ? PLAYER1_PIECE : PLAYER2_PIECE)) {
                return true;
            }
        }
    }
    return false;
}

int PiecesInDirection(short x, short y, short dx, short dy, bool player1, short board[][BOARD_SIDE]) {
    int captured = 0;
    short currpiece = (player1) ? PLAYER1_PIECE : PLAYER2_PIECE;
    while((x > -1 && x < 8) && (y > -1 && y < 8)) {
        if(board[y][x] == 0) {
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

int TotalPossibleCapture(short x, short y, bool player1, short board[][BOARD_SIDE]) {
    if(Board[y][x] != 0) {
        return 0;
    }
    
    int totalcapture = 0;
    
    //check left
    if(x-1 > -1)
        totalcapture += PiecesInDirection(x - 1, y, -1, 0, player1, board);
    
    //check right
    if(x+1 < 8)
        totalcapture += PiecesInDirection(x + 1, y, 1, 0, player1, board);
    
    //check up left
    if(x-1 > -1 && y-1 > -1)
        totalcapture += PiecesInDirection(x - 1, y - 1, -1, -1, player1, board);
    
    //check up
    if(y-1 > -1)
        totalcapture += PiecesInDirection(x , y - 1, 0, -1, player1, board);
    
    //check up right
    if(x+1 < 8 && y-1 > -1)
        totalcapture += PiecesInDirection(x + 1, y - 1, 1, -1, player1, board);
    
    //check down left
    if(x-1 > -1 && y+1 < 8)
        totalcapture += PiecesInDirection(x - 1, y + 1, -1, 1, player1, board);
    
    //check down
    if(y+1 < 8)
        totalcapture += PiecesInDirection(x, y + 1, 0, 1, player1, board);
    
    //check down right
    if(x+1 < 8 && y+1 < 8)
        totalcapture += PiecesInDirection(x + 1, y + 1, 1, 1, player1, board);
    
    return totalcapture;
}

int FlipPieces(const short x, const short y, bool player1, short board[][BOARD_SIDE]) {
    if(board[y][x] != 0) {
        return false;
    }
    
    int totalchanged = 0;
    int captured = 0;
    short currpiece = (player1) ? 1 : 2;
    //check left
    captured = PiecesInDirection(x - 1, y, -1, 0, player1, board);
    totalchanged += captured;
    for(int i = 0; i < captured; ++i) {
        board[y][x - 1 - i] = currpiece;
    }
    
    //check right
    captured = PiecesInDirection(x + 1, y, 1, 0, player1, board);
    totalchanged += captured;
    for(int i = 0; i < captured; ++i) {
        board[y][x + 1 + i] = currpiece;
    }
    
    //check up left
    captured = PiecesInDirection(x - 1, y - 1, -1, -1, player1, board);
    totalchanged += captured;
    for(int i = 0; i < captured; ++i) {
        board[y - 1 - i][x - 1 - i] = currpiece;
    }
    
    //check up
    captured = PiecesInDirection(x , y - 1, 0, -1, player1, board);
    totalchanged += captured;
    for(int i = 0; i < captured; ++i) {
        board[y - 1 - i][x] = currpiece;
    }
    
    //check up right
    captured = PiecesInDirection(x + 1, y - 1, 1, -1, player1, board);
    totalchanged += captured;
    for(int i = 0; i < captured; ++i) {
        board[y - 1 - i][x + 1 + i] = currpiece;
    }
    
    //check down left
    captured = PiecesInDirection(x - 1, y + 1, -1, 1, player1, board);
    totalchanged += captured;
    for(int i = 0; i < captured; ++i) {
        board[y + 1 + i][x - 1 - i] = currpiece;
    }
    
    //check down
    captured = PiecesInDirection(x, y + 1, 0, 1, player1, board);
    totalchanged += captured;
    for(int i = 0; i < captured; ++i) {
        board[y + 1 + i][x] = currpiece;
    }
    
    //check down right
    captured = PiecesInDirection(x + 1, y + 1, 1, 1, player1, board);
    totalchanged += captured;
    for(int i = 0; i < captured; ++i) {
        board[y + 1 + i][x + 1 + i] = currpiece;
    }
    
    return totalchanged;
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

void InitBoard(short board[][BOARD_SIDE]) {
    board[3][4] = 1;
    board[3][3] = 2;
    board[4][4] = 2;
    board[4][3] = 1;
    P1Turn = true;
}

void CopyBoard(short board[][BOARD_SIDE], short newboard[][BOARD_SIDE]) {
    for(int y = 0; y < BOARD_SIDE; ++y) {
        for(int x = 0; x < BOARD_SIDE; ++x) {
            newboard[y][x] = board[y][x];
        }
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