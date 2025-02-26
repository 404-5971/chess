#include "pieces.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to safely load an image and texture
static Texture2D safeLoadTexture(const char *path) {
  printf("Attempting to load: %s\n", path); // Debug print

  // Check if the file exists before trying to load it
  if (!FileExists(path)) {
    printf("ERROR: File does not exist: %s\n", path);
    exit(1);
  }

  Image tempImage = LoadImage(path);
  if (!tempImage.data) {
    printf("ERROR: Failed to load image: %s\n", path);
    exit(1);
  }

  Texture2D texture = LoadTextureFromImage(tempImage);
  if (texture.id == 0) {
    printf("ERROR: Failed to create texture from image: %s\n", path);
    UnloadImage(tempImage);
    exit(1);
  }

  printf("Successfully loaded: %s\n", path); // Debug print
  UnloadImage(tempImage);
  return texture;
}

struct ChessPieces loadChessPieces(void) {
  struct ChessPieces pieces;
  memset(&pieces, 0,
         sizeof(struct ChessPieces)); // Initialize all fields to zero

  // Load pieces one by one with careful error handling
  printf("Loading chess pieces...\n");

  pieces.whiteKing = safeLoadTexture(WKING);
  pieces.blackKing = safeLoadTexture(BKING);
  pieces.whiteQueen = safeLoadTexture(WQUEEN);
  pieces.blackQueen = safeLoadTexture(BQUEEN);

  // Load paired pieces
  for (int i = 0; i < 2; i++) {
    printf("Loading piece set %d of 2\n", i + 1);
    pieces.whiteRook[i] = safeLoadTexture(WROOK);
    pieces.blackRook[i] = safeLoadTexture(BROOK);
    pieces.whiteKnight[i] = safeLoadTexture(WKNIGHT);
    pieces.blackKnight[i] = safeLoadTexture(BKNIGHT);
    pieces.whiteBishop[i] = safeLoadTexture(WBISHOP);
    pieces.blackBishop[i] = safeLoadTexture(BBISHOP);
  }

  // Load pawns
  for (int i = 0; i < 8; i++) {
    printf("Loading pawn set %d of 8\n", i + 1);
    pieces.whitePawn[i] = safeLoadTexture(WPAWN);
    pieces.blackPawn[i] = safeLoadTexture(BPAWN);
  }

  printf("All chess pieces loaded successfully!\n");
  return pieces;
}

void unloadChessPieces(struct ChessPieces pieces) {
  printf("Unloading chess pieces...\n");

  // Unload single pieces
  UnloadTexture(pieces.whiteKing);
  UnloadTexture(pieces.blackKing);
  UnloadTexture(pieces.whiteQueen);
  UnloadTexture(pieces.blackQueen);

  // Unload paired pieces
  for (int i = 0; i < 2; i++) {
    UnloadTexture(pieces.whiteRook[i]);
    UnloadTexture(pieces.blackRook[i]);
    UnloadTexture(pieces.whiteKnight[i]);
    UnloadTexture(pieces.blackKnight[i]);
    UnloadTexture(pieces.whiteBishop[i]);
    UnloadTexture(pieces.blackBishop[i]);
  }

  // Unload pawns
  for (int i = 0; i < 8; i++) {
    UnloadTexture(pieces.whitePawn[i]);
    UnloadTexture(pieces.blackPawn[i]);
  }

  printf("All chess pieces unloaded successfully!\n");
}

