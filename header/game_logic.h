#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <stdbool.h>
#include "pieces.h"

// Piece types
typedef enum {
    EMPTY,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
} PieceType;

// Colors
typedef enum {
    COLOR_NONE,
    COLOR_WHITE,
    COLOR_BLACK
} ColorPieces;

// Piece structure
typedef struct {
    PieceType type;
    ColorPieces color;
    bool hasMoved;  // For pawns, kings, rooks (castling)
} Piece;

// Move structure
typedef struct {
    int fromX;
    int fromY;
    int toX;
    int toY;
} Move;

// Game state
typedef struct {
    Piece board[8][8];
    ColorPieces currentTurn;
    bool isCheck;
    bool isCheckmate;
    bool isStalemate;
    Move lastMove;
    // For en passant and castling
    bool canEnPassant;
    int enPassantColumn;
} GameState;

// Function declarations
GameState initializeGame(void);
bool isValidMove(GameState *game, int fromX, int fromY, int toX, int toY);
bool makeMove(GameState *game, Move move);
bool isInCheck(GameState *game, ColorPieces color);
bool isCheckmate(GameState *game);
bool isStalemate(GameState *game);
void getPossibleMoves(GameState *game, int x, int y, bool moves[8][8]);

#endif 