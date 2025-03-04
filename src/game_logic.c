#include "game_logic.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Initialize the game board
GameState initializeGame(void) {
    GameState game = {0};
    game.currentTurn = COLOR_WHITE;
    game.enPassantCol = -1;
    game.enPassantRow = -1;
    
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
    
    // Diagonal capture
    if (abs(fromX - toX) == 1 && toY == fromY + direction) {
        if (game->board[toY][toX].type != EMPTY && game->board[toY][toX].color != piece.color) {
            return true; // Regular diagonal capture
        }
        
        // En passant check
        // For en passant, the target square is empty, but there's an enemy pawn adjacent to our pawn
        if (game->board[toY][toX].type == EMPTY && 
            toX == game->enPassantCol && toY == game->enPassantRow) {
            Piece adjacentPiece = game->board[fromY][toX];
            // Check if the adjacent piece is an enemy pawn
            if (adjacentPiece.type == PAWN && adjacentPiece.color != piece.color) {
                return true; // Valid en passant
            }
        }
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

// Direct check to see if a king is attacked, without using isValidMove to avoid recursion
static bool isKingAttacked(GameState* game, int kingX, int kingY, ColorPieces kingColor) {
    ColorPieces opponent = (kingColor == COLOR_WHITE) ? COLOR_BLACK : COLOR_WHITE;
    
    // Check for attacks from each opponent piece
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (game->board[y][x].type != EMPTY && game->board[y][x].color == opponent) {
                Piece piece = game->board[y][x];
                int dx = abs(kingX - x);
                int dy = abs(kingY - y);
                
                switch (piece.type) {
                    case PAWN:
                        // Pawns capture diagonally
                        if (dx == 1 && ((opponent == COLOR_WHITE && y + 1 == kingY) || 
                                       (opponent == COLOR_BLACK && y - 1 == kingY))) {
                            return true;
                        }
                        break;
                    case KNIGHT:
                        // Knight's L-shape move
                        if ((dx == 1 && dy == 2) || (dx == 2 && dy == 1)) {
                            return true;
                        }
                        break;
                    case BISHOP:
                        // Bishop moves diagonally
                        if (dx == dy && isPathClear(game, x, y, kingX, kingY)) {
                            return true;
                        }
                        break;
                    case ROOK:
                        // Rook moves horizontally or vertically
                        if ((x == kingX || y == kingY) && isPathClear(game, x, y, kingX, kingY)) {
                            return true;
                        }
                        break;
                    case QUEEN:
                        // Queen combines bishop and rook moves
                        if (((dx == dy) || (x == kingX || y == kingY)) && 
                            isPathClear(game, x, y, kingX, kingY)) {
                            return true;
                        }
                        break;
                    case KING:
                        // King can capture adjacent king
                        if (dx <= 1 && dy <= 1) {
                            return true;
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }
    
    return false; // The king is not in check
}

// Function to check if a move would result in the king being in check
static bool moveWouldCauseCheck(GameState* game, int fromX, int fromY, int toX, int toY) {
    // Save current state
    Piece tempFromPiece = game->board[fromY][fromX];
    Piece tempToPiece = game->board[toY][toX];
    bool isEnPassant = false;
    
    // Check for en passant capture
    if (tempFromPiece.type == PAWN && abs(toX - fromX) == 1 && 
        game->board[toY][toX].type == EMPTY && 
        toX == game->enPassantCol && toY == game->enPassantRow) {
        isEnPassant = true;
    }
    
    // Make the move temporarily
    game->board[toY][toX] = game->board[fromY][fromX];
    game->board[fromY][fromX].type = EMPTY;
    
    // If en passant, also remove the captured pawn
    Piece capturedPawn = {EMPTY, COLOR_NONE, false};
    int capturedY = fromY;
    if (isEnPassant) {
        capturedPawn = game->board[fromY][toX];
        game->board[fromY][toX].type = EMPTY;
    }
    
    // Find king position (might have moved!)
    int kingX = -1, kingY = -1;
    ColorPieces color = tempFromPiece.color;
    
    // If we moved the king, use the new position
    if (tempFromPiece.type == KING) {
        kingX = toX;
        kingY = toY;
    } else {
        // Otherwise find the king
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
    }
    
    // Check if the king is attacked after the move
    bool isCheck = isKingAttacked(game, kingX, kingY, color);
    
    // Restore the board
    game->board[fromY][fromX] = tempFromPiece;
    game->board[toY][toX] = tempToPiece;
    
    // Restore captured pawn if it was en passant
    if (isEnPassant) {
        game->board[fromY][toX] = capturedPawn;
    }
    
    return isCheck;
}

// Add helper function for castling
static bool canCastle(GameState* game, int fromX, int fromY, int toX, int toY) {
    Piece piece = game->board[fromY][fromX];
    
    if (piece.type != KING) return false;
    if (piece.hasMoved) return false;
    if (fromY != toY || abs(toX - fromX) != 2) return false;
    
    // Check if king is in check
    if (isKingAttacked(game, fromX, fromY, piece.color)) return false;
    
    bool isKingside = (toX > fromX);
    int rookX = isKingside ? 7 : 0;
    
    Piece rook = game->board[fromY][rookX];
    if (rook.type != ROOK || rook.hasMoved) return false;
    
    int step = isKingside ? 1 : -1;
    for (int x = fromX + step; x != rookX; x += step) {
        if (game->board[fromY][x].type != EMPTY) return false;
    }
    
    // Check if king passes through check
    int midX = fromX + step;
    if (moveWouldCauseCheck(game, fromX, fromY, midX, fromY)) return false;
    
    return true;
}


// Main move validation function
bool isValidMove(GameState* game, int fromX, int fromY, int toX, int toY) {
    if (!isInBoard(fromX, fromY) || !isInBoard(toX, toY)) {
        return false;
    }
    
    Piece piece = game->board[fromY][fromX];
    
    // Can't move empty square
    if (piece.type == EMPTY) {
        return false;
    }
    
    // Can't move opponent's pieces
    if (piece.color != game->currentTurn) {
        return false;
    }
    
    // Can't capture own pieces
    Piece destPiece = game->board[toY][toX];
    if (destPiece.type != EMPTY && destPiece.color == piece.color) {
        return false;
    }
    
    // Piece-specific move validation
    bool validPieceMove = false;
    
    switch(piece.type) {
        case PAWN:
            validPieceMove = isValidPawnMove(game, fromX, fromY, toX, toY);
            break;
        case KNIGHT:
            validPieceMove = isValidKnightMove(game, fromX, fromY, toX, toY);
            break;
        case BISHOP:
            validPieceMove = isValidBishopMove(game, fromX, fromY, toX, toY);
            break;
        case ROOK:
            validPieceMove = isValidRookMove(game, fromX, fromY, toX, toY);
            break;
        case QUEEN:
            validPieceMove = isValidQueenMove(game, fromX, fromY, toX, toY);
            break;
        case KING:
            if (abs(toX - fromX) == 2) {
                return canCastle(game, fromX, fromY, toX, toY);
            } else {
                validPieceMove = isValidKingMove(game, fromX, fromY, toX, toY);
            }
            break;
        default:
            return false;
    }
    
    if (!validPieceMove) {
        return false;
    }
    
    // Check if the move would leave or put the king in check
    if (moveWouldCauseCheck(game, fromX, fromY, toX, toY)) {
        return false;
    }
    
    return true;
}
// Function to check if a player is in check
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
    
    if (kingX == -1) return false; // No king found
    
    return isKingAttacked(game, kingX, kingY, color);
}

// Make a move and update game state
bool makeMove(GameState* game, Move move) {
    if (!isValidMove(game, move.fromX, move.fromY, move.toX, move.toY)) {
        return false;
    }
    
    Piece piece = game->board[move.fromY][move.fromX];
    bool isEnPassant = false;
    
    // Check for en passant capture
    if (piece.type == PAWN && abs(move.toX - move.fromX) == 1 && 
        game->board[move.toY][move.toX].type == EMPTY &&
        move.toX == game->enPassantCol && move.toY == game->enPassantRow) {
        isEnPassant = true;
    }
    
    // Save the old en passant state
    int oldEnPassantCol = game->enPassantCol;
    int oldEnPassantRow = game->enPassantRow;
    
    // Reset en passant flag for the next move
    game->enPassantCol = -1;
    game->enPassantRow = -1;
    
    // Check for pawn moving two squares (possible en passant next move)
    if (piece.type == PAWN && abs(move.toY - move.fromY) == 2) {
        game->enPassantCol = move.fromX;
        game->enPassantRow = (move.fromY + move.toY) / 2; // The square the pawn skipped over
    }
    
    // Handle en passant capture
    if (isEnPassant) {
        game->board[move.fromY][move.toX].type = EMPTY; // Remove the captured pawn
    }

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

// bool isCheckmate(GameState* game) {
//     // If not in check, it can't be checkmate
//     if (!game->isCheck) {
//         return false;
//     }
    
    // Try every possible move for the current player
    // for (int fromY = 0; fromY < 8; fromY++) {
    //     for (int fromX = 0; fromX < 8; fromX++) {
    //         // Only check pieces of current player
    //         if (game->board[fromY][fromX].type != EMPTY && 
    //             game->board[fromY][fromX].color == game->currentTurn) {
                
    //             // Try moving to every square
    //             for (int toY = 0; toY < 8; toY++) {
    //                 for (int toX = 0; toX < 8; toX++) {
                        // Try the move directly with isValidMove
                        // if (isValidMove(game, fromX, fromY, toX, toY)) {
                        //     // If we found a valid move, it's not checkmate
                        //     return false;
                        // }
                    // }
                // }
            // }
        // }
    // }
    
    // If no legal moves found, it's checkmate
    // return true;
// }

void getPossibleMoves(GameState* game, int x, int y, bool moves[8][8]) {
    // Clear the moves array
    memset(moves, 0, 64 * sizeof(bool));
    
    // Check if the square contains a piece of the current player
    if (game->board[y][x].type == EMPTY || 
        game->board[y][x].color != game->currentTurn) {
        return;
    }
    
    // Try every possible destination
    for (int toY = 0; toY < 8; toY++) {
        for (int toX = 0; toX < 8; toX++) {
            if (isValidMove(game, x, y, toX, toY)) {
                moves[toY][toX] = true;
            }
        }
    }
}

bool isKingCheckmated(GameState* game) {
    // Check if the king is in check
    if (!isInCheck(game, game->currentTurn)) {
        return false; // Not checkmated if the king is not in check
    }

    // Check for any valid moves for the current player
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Piece piece = game->board[y][x];
            if (piece.color == game->currentTurn) {
                // Check all possible moves for this piece
                for (int toY = 0; toY < 8; toY++) {
                    for (int toX = 0; toX < 8; toX++) {
                        if (isValidMove(game, x, y, toX, toY)) {
                            return false; // Found a valid move, not checkmated
                        }
                    }
                }
            }
        }
    }

    return true; // No valid moves found, player is checkmated
}
