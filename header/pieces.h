#ifndef HEAD_H
#define HEAD_H

#include <raylib.h>

#pragma message "ASSET_PATH is: " ASSET_PATH

// Image path definitions
#define WKING ASSET_PATH "images/whiteKing.png"
#define BKING ASSET_PATH "images/blackKing.png"
#define WROOK ASSET_PATH "images/whiteRook.png"
#define BROOK ASSET_PATH "images/blackRook.png"
#define WQUEEN ASSET_PATH "images/whiteQueen.png"
#define BQUEEN ASSET_PATH "images/blackQueen.png"
#define WPAWN ASSET_PATH "images/whitePawn.png"
#define BPAWN ASSET_PATH "images/blackPawn.png"
#define WKNIGHT ASSET_PATH "images/whiteKnight.png"
#define BKNIGHT ASSET_PATH "images/blackKnight.png"
#define WBISHOP ASSET_PATH "images/whiteBishop.png"
#define BBISHOP ASSET_PATH "images/blackBishop.png"

struct ChessPieces {
  Texture2D whiteKing;
  Texture2D whiteRook[2];
  Texture2D whiteBishop[2];
  Texture2D whiteKnight[2];
  Texture2D whitePawn[8];
  Texture2D whiteQueen;

  Texture2D blackKing;
  Texture2D blackRook[2];
  Texture2D blackBishop[2];
  Texture2D blackKnight[2];
  Texture2D blackPawn[8];
  Texture2D blackQueen;
};

// Function prototypes
struct ChessPieces loadChessPieces(void);
void unloadChessPieces(struct ChessPieces pieces);

#endif // HEAD_H

