#include "game_logic.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Initialize the game board
GameState initializeGame(void) {
    GameState game = {0};
    game.currentTurn = COLOR_WHITE;
    
    // Set up pawns
    for(int i = 0; i < 8; i++) {
        game.board[1][i] = (Piece){PAWN, COLOR_BLACK, false};
        game.board[6][i] = (Piece){PAWN, COLOR_WHITE, false};
    }
    
    // Set up other pieces with hasMoved = false
    // Black pieces
    game.board[0][0] = game.board[0][7] = (Piece){ROOK, COLOR_BLACK, false};
    game.board[0][1] = game.board[0][6] = (Piece){KNIGHT, COLOR_BLACK, false};
    game.board[0][2] = game.board[0][5] = (Piece){BISHOP, COLOR_BLACK, false};
    game.board[0][3] = (Piece){QUEEN, COLOR_BLACK, false};
    game.board[0][4] = (Piece){KING, COLOR_BLACK, false};
    
    // White pieces
    game.board[7][0] = game.board[7][7] = (Piece){ROOK, COLOR_WHITE, false};
    game.board[7][1] = game.board[7][6] = (Piece){KNIGHT, COLOR_WHITE, false};
    game.board[7][2] = game.board[7][5] = (Piece){BISHOP, COLOR_WHITE, false};
    game.board[7][3] = (Piece){QUEEN, COLOR_WHITE, false};
    game.board[7][4] = (Piece){KING, COLOR_WHITE, false};
    
    return game;
}

// Check if a position is within the board
static bool isInBoard(int x, int y) {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

static bool isPathClear(GameState* game, int fromX, int fromY, int toX, int toY) {
    int dx = (toX - fromX) ? (toX - fromX) / abs(toX - fromX) : 0;
    int dy = (toY - fromY) ? (toY - fromY) / abs(toY - fromY) : 0;
    
    int x = fromX + dx;
    int y = fromY + dy;
    
    while (x != toX || y != toY) {
        if (game->board[y][x].type != EMPTY) {
            return false;
        }
        x += dx;
        y += dy;
    }
    return true;
}

// Check if a move is valid for a pawn
static bool isValidPawnMove(GameState* game, int fromX, int fromY, int toX, int toY) {
    Piece piece = game->board[fromY][fromX];
    int direction = (piece.color == COLOR_WHITE) ? -1 : 1;
    int startRank = (piece.color == COLOR_WHITE) ? 6 : 1;
    
    // Basic one square forward move
    if (fromX == toX && toY == fromY + direction) {
        return game->board[toY][toX].type == EMPTY;
    }
    
    // Initial two square move
    if (fromX == toX && fromY == startRank && toY == fromY + 2 * direction) {
        return game->board[toY][toX].type == EMPTY && 
               game->board[fromY + direction][fromX].type == EMPTY;
    }
    
    // Capture moves
    if (abs(fromX - toX) == 1 && toY == fromY + direction) {
        return game->board[toY][toX].type != EMPTY && 
               game->board[toY][toX].color != piece.color;
    }
    
    return false;
}

static bool isValidKnightMove(GameState* game, int fromX, int fromY, int toX, int toY) {
    (void)game; // Suppress unused parameter warning
    int dx = abs(toX - fromX);
    int dy = abs(toY - fromY);
    return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}

static bool isValidBishopMove(GameState* game, int fromX, int fromY, int toX, int toY) {
    int dx = abs(toX - fromX);
    int dy = abs(toY - fromY);
    return dx == dy && isPathClear(game, fromX, fromY, toX, toY);
}

static bool isValidRookMove(GameState* game, int fromX, int fromY, int toX, int toY) {
    return ((fromX == toX || fromY == toY) && 
            isPathClear(game, fromX, fromY, toX, toY));
}

static bool isValidQueenMove(GameState* game, int fromX, int fromY, int toX, int toY) {
    int dx = abs(toX - fromX);
    int dy = abs(toY - fromY);
    return ((dx == dy || fromX == toX || fromY == toY) && 
            isPathClear(game, fromX, fromY, toX, toY));
}

static bool isValidKingMove(GameState* game, int fromX, int fromY, int toX, int toY) {
    (void)game; // Suppress unused parameter warning
    int dx = abs(toX - fromX);
    int dy = abs(toY - fromY);
    return dx <= 1 && dy <= 1;
}

// Add helper function for castling
static bool canCastle(GameState* game, int fromX, int fromY, int toX, int toY) {
    Piece piece = game->board[fromY][fromX];
    
    if (piece.type != KING) return false;
    if (piece.hasMoved) return false;
    if (fromY != toY || abs(toX - fromX) != 2) return false;
    
    bool isKingside = (toX > fromX);
    int rookX = isKingside ? 7 : 0;
    
    Piece rook = game->board[fromY][rookX];
    if (rook.type != ROOK || rook.hasMoved) return false;
    
    int step = isKingside ? 1 : -1;
    for (int x = fromX + step; x != rookX; x += step) {
        if (game->board[fromY][x].type != EMPTY) return false;
    }
    
    if (isInCheck(game, piece.color)) return false;
    
    int midX = fromX + step;
    Piece tempPiece = game->board[fromY][midX];
    game->board[fromY][midX] = piece;
    game->board[fromY][fromX].type = EMPTY;
    bool passesThroughCheck = isInCheck(game, piece.color);
    game->board[fromY][fromX] = piece;
    game->board[fromY][midX] = tempPiece;
    
    if (passesThroughCheck) return false;
    
    return true;
}

// Main move validation function
bool isValidMove(GameState* game, int fromX, int fromY, int toX, int toY) {
    if (!isInBoard(fromX, fromY) || !isInBoard(toX, toY)) {
        return false;
    }
    
    Piece piece = game->board[fromY][fromX];
    
    // Can't move empty square or opponent's pieces
    if (piece.type == EMPTY || piece.color != game->currentTurn) {
        return false;
    }
    
    // Can't capture own pieces
    if (game->board[toY][toX].type != EMPTY && 
        game->board[toY][toX].color == piece.color) {
        return false;
    }
    
    // Check for castling
    if (piece.type == KING && abs(toX - fromX) == 2) {
        return canCastle(game, fromX, fromY, toX, toY);
    }
    
    // Piece-specific move validation
    switch(piece.type) {
        case PAWN:
            return isValidPawnMove(game, fromX, fromY, toX, toY);
        case KNIGHT:
            return isValidKnightMove(game, fromX, fromY, toX, toY);
        case BISHOP:
            return isValidBishopMove(game, fromX, fromY, toX, toY);
        case ROOK:
            return isValidRookMove(game, fromX, fromY, toX, toY);
        case QUEEN:
            return isValidQueenMove(game, fromX, fromY, toX, toY);
        case KING:
            return isValidKingMove(game, fromX, fromY, toX, toY);
        default:
            return false;
    }
}

bool isInCheck(GameState* game, ColorPieces color) {
    // Find king position
    int kingX = -1, kingY = -1;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (game->board[y][x].type == KING && 
                game->board[y][x].color == color) {
                kingX = x;
                kingY = y;
                break;
            }
        }
        if (kingX != -1) break;
    }
    
    // Check if any opponent piece can capture the king
    ColorPieces opponent = (color == COLOR_WHITE) ? COLOR_BLACK : COLOR_WHITE;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (game->board[y][x].color == opponent) {
                if (isValidMove(game, x, y, kingX, kingY)) {
                    return true;
                }
            }
        }
    }
    return false;
}

// Make a move and update game state
bool makeMove(GameState* game, Move move) {
    if (!isValidMove(game, move.fromX, move.fromY, move.toX, move.toY)) {
        return false;
    }
    
    Piece piece = game->board[move.fromY][move.fromX];
    
    // Handle castling
    if (piece.type == KING && abs(move.toX - move.fromX) == 2) {
        bool isKingside = (move.toX > move.fromX);
        int rookFromX = isKingside ? 7 : 0;
        int rookToX = isKingside ? move.toX - 1 : move.toX + 1;
        
        // Move rook
        game->board[move.toY][rookToX] = game->board[move.toY][rookFromX];
        game->board[move.toY][rookFromX].type = EMPTY;
        game->board[move.toY][rookToX].hasMoved = true;
    }

    // Make the move
    game->board[move.toY][move.toX] = game->board[move.fromY][move.fromX];
    game->board[move.fromY][move.fromX].type = EMPTY;
    
    // Update hasMoved flag
    game->board[move.toY][move.toX].hasMoved = true;

    // Switch turns
    game->currentTurn = (game->currentTurn == COLOR_WHITE) ? COLOR_BLACK : COLOR_WHITE;
    
    // Check for check/checkmate on opponent
    game->isCheck = isInCheck(game, game->currentTurn);
    
    return true;
}

bool isCheckmate(GameState* game) {
    // If not in check, it can't be checkmate
    if (!isInCheck(game, game->currentTurn)) {
        return false;
    }
    
    // Try every possible move for the current player
    for (int fromY = 0; fromY < 8; fromY++) {
        for (int fromX = 0; fromX < 8; fromX++) {
            // Only check pieces of current player
            if (game->board[fromY][fromX].type != EMPTY && 
                game->board[fromY][fromX].color == game->currentTurn) {
                
                // Try moving to every square
                for (int toY = 0; toY < 8; toY++) {
                    for (int toX = 0; toX < 8; toX++) {
                        // Create temporary move
                        Move testMove = {fromX, fromY, toX, toY};
                        
                        // Save current board state
                        Piece tempBoard[8][8];
                        memcpy(tempBoard, game->board, sizeof(tempBoard));
                        
                        // Try the move
                        if (makeMove(game, testMove)) {
                            // If move was successful, restore board and return false (not checkmate)
                            memcpy(game->board, tempBoard, sizeof(tempBoard));
                            return false;
                        }
                        
                        // Restore board state
                        memcpy(game->board, tempBoard, sizeof(tempBoard));
                    }
                }
            }
        }
    }
    
    // If no legal moves found, it's checkmate
    return true;
}

void getPossibleMoves(GameState* game, int x, int y, bool moves[8][8]) {
    // Clear the moves array
    memset(moves, 0, 64 * sizeof(bool));
    
    // Try every possible destination
    for (int toY = 0; toY < 8; toY++) {
        for (int toX = 0; toX < 8; toX++) {
            // Create temporary move
            Move testMove = {x, y, toX, toY};
            
            // Save current board state
            Piece tempBoard[8][8];
            memcpy(tempBoard, game->board, sizeof(tempBoard));
            
            // Try the move
            if (makeMove(game, testMove)) {
                moves[toY][toX] = true;
                // Restore board state
                memcpy(game->board, tempBoard, sizeof(tempBoard));
            }
            
            // Restore board state
            memcpy(game->board, tempBoard, sizeof(tempBoard));
        }
    }
} 
