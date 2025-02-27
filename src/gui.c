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
  // Initialize window first
  InitWindow(WIDTH, HEIGHT, "CHESS_GAME");
  if (!IsWindowReady()) {
    printf("Failed to initialize window\n");
    return;
  }

  ShowCursor();

  // Load icon after window initialization
  Image icon = LoadImage(ICON);
  if (!icon.data) {
    printf("Failed to load icon\n");
    CloseWindow();
    return;
  }
  SetWindowIcon(icon);

  struct Board *a = malloc(sizeof(struct Board));
  if (!a) {
    printf("Failed to allocate board memory\n");
    UnloadImage(icon);
    CloseWindow();
    return;
  }

  GameState *gameState = malloc(sizeof(GameState));
  if (!gameState) {
    printf("Failed to allocate game state memory\n");
    free(a);
    UnloadImage(icon);
    CloseWindow();
    return;
  }

  // Load pieces after window initialization
  struct ChessPieces pieces = loadChessPieces();
  *gameState = initializeGame();

  int width = 80;
  int height = 80;
  int posX = 0;
  int posY = 0;

  // Add variables for piece dragging
  Vector2 mousePosition = {0};
  bool isDragging = false;
  int selectedX = -1, selectedY = -1;
  Texture2D* draggedPiece = NULL;

  // Add variables for checkmate screen
  bool showCheckmateScreen = false;
  float checkmateAlpha = 0.0f;
  const float flashSpeed = 2.0f;
  ColorPieces winner = COLOR_NONE;

  if (IsWindowReady()) {
    while (!WindowShouldClose()) {
      mousePosition = GetMousePosition();
      
      // Handle mouse input
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Convert mouse position to board coordinates
        int boardX = mousePosition.x / width;
        int boardY = mousePosition.y / height;
        
        if (boardX >= 0 && boardX < 8 && boardY >= 0 && boardY < 8) {
          // Select piece if it belongs to current player
          if (gameState->board[boardY][boardX].type != EMPTY &&
              gameState->board[boardY][boardX].color == gameState->currentTurn) {
            selectedX = boardX;
            selectedY = boardY;
            isDragging = true;
          }
        }
      }
      
      if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && isDragging) {
        // Convert release position to board coordinates
        int toX = mousePosition.x / width;
        int toY = mousePosition.y / height;
        
        if (toX >= 0 && toX < 8 && toY >= 0 && toY < 8) {
          // Try to make the move
          Move move = {selectedX, selectedY, toX, toY};
          makeMove(gameState, move);
        }
        
        // Reset drag state
        isDragging = false;
        selectedX = -1;
        selectedY = -1;
      }

      BeginDrawing();
      ClearBackground(WHITE);

      // Draw board
      for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
          posX = col * width;
          posY = row * height;

          // Draw squares
          if ((row + col) % 2 == 0) {
            DrawRectangle(posX, posY, width, height, BROWN);
          } else {
            DrawRectangle(posX, posY, width, height, LIGHTGRAY);
          }

          // Draw pieces (skip dragged piece)
          if (!(isDragging && col == selectedX && row == selectedY)) {
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
      }

      // Draw dragged piece at mouse position if dragging
      if (isDragging) {
        Piece piece = gameState->board[selectedY][selectedX];
        float drawX = mousePosition.x - width/2;
        float drawY = mousePosition.y - height/2;
        
        // Draw the dragged piece centered on mouse
        switch (piece.type) {
          case KING:
            DrawTexture(piece.color == COLOR_WHITE ? pieces.whiteKing : pieces.blackKing,
                      drawX, drawY, WHITE);
            break;
          case QUEEN:
            DrawTexture(piece.color == COLOR_WHITE ? pieces.whiteQueen : pieces.blackQueen,
                      drawX, drawY, WHITE);
            break;
          case ROOK:
            DrawTexture(piece.color == COLOR_WHITE ? pieces.whiteRook[selectedX == 0 ? 0 : 1] : pieces.blackRook[selectedX == 0 ? 0 : 1],
                      drawX, drawY, WHITE);
            break;
          case BISHOP:
            DrawTexture(piece.color == COLOR_WHITE ? pieces.whiteBishop[selectedX == 2 ? 0 : 1] : pieces.blackBishop[selectedX == 2 ? 0 : 1],
                      drawX, drawY, WHITE);
            break;
          case KNIGHT:
            DrawTexture(piece.color == COLOR_WHITE ? pieces.whiteKnight[selectedX == 1 ? 0 : 1] : pieces.blackKnight[selectedX == 1 ? 0 : 1],
                      drawX, drawY, WHITE);
            break;
          case PAWN:
            DrawTexture(piece.color == COLOR_WHITE ? pieces.whitePawn[selectedX] : pieces.blackPawn[selectedX],
                      drawX, drawY, WHITE);
            break;
          default:
            break;
        }
      }

      // Check for checkmate after each move
      if (isCheckmate(gameState)) {
        showCheckmateScreen = true;
        winner = (gameState->currentTurn == COLOR_WHITE) ? COLOR_BLACK : COLOR_WHITE;
      }

      // Draw checkmate screen
      if (showCheckmateScreen) {
        checkmateAlpha += flashSpeed * GetFrameTime();
        if (checkmateAlpha > 1.0f) checkmateAlpha = 0.0f;

        Color overlayColor = ColorAlpha(BLACK, 0.7f);
        DrawRectangle(0, 0, WIDTH, HEIGHT, overlayColor);

        const char* winnerText = (winner == COLOR_WHITE) ? "White Wins!" : "Black Wins!";
        const char* checkmateText = "CHECKMATE!";
        const char* pressKeyText = "Press ENTER to restart or ESC to quit";

        // Calculate text positions for centering
        int winnerFontSize = 60;
        int checkmateFontSize = 80;
        int pressKeyFontSize = 20;

        Vector2 winnerPos = {
          (WIDTH - MeasureText(winnerText, winnerFontSize)) / 2,
          HEIGHT / 2 - 80
        };
        Vector2 checkmatePos = {
          (WIDTH - MeasureText(checkmateText, checkmateFontSize)) / 2,
          HEIGHT / 2
        };
        Vector2 pressKeyPos = {
          (WIDTH - MeasureText(pressKeyText, pressKeyFontSize)) / 2,
          HEIGHT / 2 + 80
        };

        Color flashColor = ColorAlpha(WHITE, checkmateAlpha);
        DrawText(winnerText, winnerPos.x, winnerPos.y, winnerFontSize, WHITE);
        DrawText(checkmateText, checkmatePos.x, checkmatePos.y, checkmateFontSize, flashColor);
        DrawText(pressKeyText, pressKeyPos.x, pressKeyPos.y, pressKeyFontSize, WHITE);

        // Handle restart or quit
        if (IsKeyPressed(KEY_ENTER)) {
          // Reset game
          *gameState = initializeGame();
          showCheckmateScreen = false;
        } else if (IsKeyPressed(KEY_ESCAPE)) {
          break;
        }
      }

      EndDrawing();
    }
  }

  // Cleanup in reverse order
  unloadChessPieces(pieces);
  UnloadImage(icon);
  free(gameState);
  free(a);
  CloseWindow();
}
