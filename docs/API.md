# Chess-Raylib API Reference

## Game State Management

### `GameState initializeGame(void)`
Initializes a new chess game with default piece positions.

### `bool isValidMove(GameState *game, int fromX, int fromY, int toX, int toY)`
Validates if a move is legal according to chess rules.

### `bool makeMove(GameState *game, Move move)`
Executes a move and updates game state.

### `bool isInCheck(GameState *game, ColorPieces color)`
Checks if specified color is in check.

## GUI Functions

### `void gameState(void)`
Main game loop and window management.

### `struct ChessPieces loadChessPieces(void)`
Loads all piece textures and resources.

## Data Structures

### GameState
```c
typedef struct {
    Piece board[8][8];
    ColorPieces currentTurn;
    bool isCheck;
    bool isCheckmate;
    Move lastMove;
} GameState;
```

### Move
```c
typedef struct {
    int fromX;
    int fromY;
    int toX;
    int toY;
} Move;
``` 