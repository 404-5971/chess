#include "gui.h"
#include "game_logic.h"
#include "pieces.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  // Initialize audio with error checking
  InitAudioDevice();
  if (!IsAudioDeviceReady()) {
    printf("Failed to initialize audio device\n");
    CloseWindow();
    return;
  }

  // Load sounds with error checking
  Sound moveSound = LoadSound(MOVE_SOUND);
  if (moveSound.frameCount == 0) {
    printf("Failed to load move sound\n");
    CloseAudioDevice();
    CloseWindow();
    return;
  }

  Sound captureSound = LoadSound(CAPTURE_SOUND);
  if (captureSound.frameCount == 0) {
    printf("Failed to load capture sound\n");
    UnloadSound(moveSound);
    CloseAudioDevice();
    CloseWindow();
    return;
  }

  // Set sound volume
  SetSoundVolume(moveSound, 0.3f);
  SetSoundVolume(captureSound, 0.7f);

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

  // Variables for drag and drop
  Vector2 mousePosition = {0};
  bool isDragging = false;
  int draggedX = -1, draggedY = -1;
  Piece draggedPiece = {EMPTY, COLOR_NONE, false};
  Vector2 dragOffset = {0};

  // Add variables for checkmate screen
  bool showCheckmateScreen = false;
  float checkmateAlpha = 0.0f;
  const float flashSpeed = 2.0f;
  ColorPieces winner = COLOR_NONE;

  SetTargetFPS(FPS);

  if (IsWindowReady()) {
    while (!WindowShouldClose()) {
      mousePosition = GetMousePosition();
      
      // Handle piece dragging
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        int boardX = mousePosition.x / width;
        int boardY = mousePosition.y / height;
        
        if (boardX >= 0 && boardX < 8 && boardY >= 0 && boardY < 8) {
          Piece piece = gameState->board[boardY][boardX];
          if (piece.type != EMPTY && piece.color == gameState->currentTurn) {
            isDragging = true;
            draggedX = boardX;
            draggedY = boardY;
            draggedPiece = piece;
            // Calculate offset from piece center
            dragOffset.x = (boardX * width + width/2) - mousePosition.x;
            dragOffset.y = (boardY * height + height/2) - mousePosition.y;
          }
        }
      }

      if (isDragging && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        // Calculate drop position
        int dropX = mousePosition.x / width;
        int dropY = mousePosition.y / height;
        
        if (dropX >= 0 && dropX < 8 && dropY >= 0 && dropY < 8) {
          Move move = {draggedX, draggedY, dropX, dropY};
          bool isCapture = gameState->board[dropY][dropX].type != EMPTY;
          
          if (makeMove(gameState, move)) {
            // Always play a sound on successful move
            if (isCapture) {
              StopSound(captureSound);  // Stop any currently playing sound
              PlaySound(captureSound);
            } else {
              StopSound(moveSound);     // Stop any currently playing sound
              PlaySound(moveSound);
            }
          }
        }
        
        // Reset drag state
        isDragging = false;
        draggedX = -1;
        draggedY = -1;
        draggedPiece.type = EMPTY;
      }

      BeginDrawing();
      ClearBackground(WHITE);

      // Draw board
      for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
          posX = col * width;
          posY = row * height;

          // Draw squares
          Color squareColor = ((row + col) % 2 == 0) ? BROWN : LIGHTGRAY;
          
          DrawRectangle(posX, posY, width, height, squareColor);

          // Draw pieces (except dragged piece)
          if (!(isDragging && col == draggedX && row == draggedY)) {
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

      // Draw dragged piece
      if (isDragging) {
        float drawX = mousePosition.x + dragOffset.x - width/2;
        float drawY = mousePosition.y + dragOffset.y - height/2;
        
        switch (draggedPiece.type) {
        case KING:
          DrawTexture(draggedPiece.color == COLOR_WHITE ? pieces.whiteKing
                                                       : pieces.blackKing,
                    drawX, drawY, WHITE);
          break;
        case QUEEN:
          DrawTexture(draggedPiece.color == COLOR_WHITE ? pieces.whiteQueen
                                                       : pieces.blackQueen,
                    drawX, drawY, WHITE);
          break;
        case ROOK:
          DrawTexture(draggedPiece.color == COLOR_WHITE
                        ? pieces.whiteRook[draggedX == 0 ? 0 : 1]
                        : pieces.blackRook[draggedX == 0 ? 0 : 1],
                        drawX, drawY, WHITE);
          break;
        case BISHOP:
          DrawTexture(draggedPiece.color == COLOR_WHITE
                        ? pieces.whiteBishop[draggedX == 2 ? 0 : 1]
                        : pieces.blackBishop[draggedX == 2 ? 0 : 1],
                        drawX, drawY, WHITE);
          break;
        case KNIGHT:
          DrawTexture(draggedPiece.color == COLOR_WHITE
                        ? pieces.whiteKnight[draggedX == 1 ? 0 : 1]
                        : pieces.blackKnight[draggedX == 1 ? 0 : 1],
                        drawX, drawY, WHITE);
          break;
        case PAWN:
          DrawTexture(draggedPiece.color == COLOR_WHITE ? pieces.whitePawn[draggedX]
                                                       : pieces.blackPawn[draggedX],
                    drawX, drawY, WHITE);
          break;
        default:
          break;
        }
      }

      // Check for checkmate
      if (isCheckmate(gameState)) {
        if (!showCheckmateScreen) {
          showCheckmateScreen = true;
          winner = (gameState->currentTurn == COLOR_WHITE) ? COLOR_BLACK : COLOR_WHITE;
        }
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

        if (IsKeyPressed(KEY_ENTER)) {
          *gameState = initializeGame();
          showCheckmateScreen = false;
        }
      }

      EndDrawing();
    }
  }

  // Cleanup audio properly
  StopSound(moveSound);
  StopSound(captureSound);
  UnloadSound(moveSound);
  UnloadSound(captureSound);
  CloseAudioDevice();
  unloadChessPieces(pieces);
  UnloadImage(icon);
  free(gameState);
  free(a);
  CloseWindow();
}
