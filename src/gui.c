#include "gui.h"
#include "game_logic.h"
#include "pieces.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

struct Board {
  Rectangle black[32];
  Rectangle grey[32];
};

void gameState(void) {
  Image icon = LoadImage(ICON);
  struct Board *a = malloc(sizeof(struct Board));
  struct ChessPieces pieces = loadChessPieces();
  GameState *gameState = malloc(sizeof(GameState));
  *gameState = initializeGame();

  int width = 80;
  int height = 80;
  int posX = 0;
  int posY = 0;

  InitWindow(WIDTH, HEIGHT, "CHESS_GAME");
  ShowCursor();
  SetWindowIcon(icon);

  if (IsWindowReady()) {
    while (!WindowShouldClose()) {
      BeginDrawing();
      ClearBackground(WHITE);

      // Draw board
      for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
          posX = col * width;
          posY = row * height;

          // Draw squares
          if ((row + col) % 2 == 0) {
            DrawRectangle(posX, posY, width, height, BLACK);
          } else {
            DrawRectangle(posX, posY, width, height, GRAY);
          }

          // Draw pieces based on game state
          Piece piece = gameState->board[row][col];
          if (piece.type != EMPTY) {
            switch (piece.type) {
            case KING:
              DrawTexture(piece.color == COLOR_WHITE ? pieces.whiteKing
                                                     : pieces.blackKing,
                          posX, posY, WHITE);
              break;
            case QUEEN:
              DrawTexture(piece.color == COLOR_WHITE ? pieces.whiteQueen
                                                     : pieces.blackQueen,
                          posX, posY, WHITE);
              break;
            case ROOK:
              DrawTexture(piece.color == COLOR_WHITE
                              ? pieces.whiteRook[col == 0 ? 0 : 1]
                              : pieces.blackRook[col == 0 ? 0 : 1],
                          posX, posY, WHITE);
              break;
            case BISHOP:
              DrawTexture(piece.color == COLOR_WHITE
                              ? pieces.whiteBishop[col == 2 ? 0 : 1]
                              : pieces.blackBishop[col == 2 ? 0 : 1],
                          posX, posY, WHITE);
              break;
            case KNIGHT:
              DrawTexture(piece.color == COLOR_WHITE
                              ? pieces.whiteKnight[col == 1 ? 0 : 1]
                              : pieces.blackKnight[col == 1 ? 0 : 1],
                          posX, posY, WHITE);
              break;
            case PAWN:
              DrawTexture(piece.color == COLOR_WHITE ? pieces.whitePawn[col]
                                                     : pieces.blackPawn[col],
                          posX, posY, WHITE);
              break;
            default:
              break;
            }
          }
        }
      }

      EndDrawing();
    }
  }

  // Cleanup
  UnloadImage(icon);
  free(a);
  free(gameState);
  // TODO: Add UnloadTexture calls for all pieces
  CloseWindow();
}
