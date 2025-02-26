#include "pieces.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to safely load an image and texture with cleanup on failure
static Texture2D safeLoadTexture(const char *path, struct ChessPieces *pieces) {
    printf("\n=== Loading Texture ===\n");
    printf("Path: %s\n", path);
    
    Texture2D texture = {0};

    if (!IsWindowReady()) {
        printf("FATAL: Window not initialized\n");
        unloadChessPieces(*pieces);
        exit(1);
    }

    if (!path) {
        printf("FATAL: Null path provided\n");
        unloadChessPieces(*pieces);
        exit(1);
    }

    if (!FileExists(path)) {
        printf("FATAL: File does not exist at path: %s\n", path);
        unloadChessPieces(*pieces);
        exit(1);
    }

    Image tempImage = LoadImage(path);
    if (!tempImage.data) {
        printf("FATAL: Failed to load image data\n");
        unloadChessPieces(*pieces);
        exit(1);
    }

    printf("Image loaded successfully: %dx%d\n", tempImage.width, tempImage.height);

    // Try to load texture directly without resizing
    texture = LoadTextureFromImage(tempImage);
    
    // Always unload the image after creating texture
    UnloadImage(tempImage);

    if (texture.id == 0) {
        printf("FATAL: Failed to create texture\n");
        unloadChessPieces(*pieces);
        exit(1);
    }

    printf("Texture created successfully (ID: %u)\n", texture.id);
    return texture;
}

struct ChessPieces loadChessPieces(void) {
    struct ChessPieces pieces = {0};

    if (!IsWindowReady()) {
        printf("FATAL: Cannot load textures before window initialization\n");
        exit(1);
    }

    // Load single pieces first
    pieces.whiteKing = safeLoadTexture(WKING, &pieces);
    pieces.blackKing = safeLoadTexture(BKING, &pieces);
    pieces.whiteQueen = safeLoadTexture(WQUEEN, &pieces);
    pieces.blackQueen = safeLoadTexture(BQUEEN, &pieces);

    // Load paired pieces
    for (int i = 0; i < 2; i++) {
        pieces.whiteRook[i] = safeLoadTexture(WROOK, &pieces);
        pieces.blackRook[i] = safeLoadTexture(BROOK, &pieces);
        pieces.whiteKnight[i] = safeLoadTexture(WKNIGHT, &pieces);
        pieces.blackKnight[i] = safeLoadTexture(BKNIGHT, &pieces);
        pieces.whiteBishop[i] = safeLoadTexture(WBISHOP, &pieces);
        pieces.blackBishop[i] = safeLoadTexture(BBISHOP, &pieces);
    }

    // Load pawns
    for (int i = 0; i < 8; i++) {
        pieces.whitePawn[i] = safeLoadTexture(WPAWN, &pieces);
        pieces.blackPawn[i] = safeLoadTexture(BPAWN, &pieces);
    }

    return pieces;
}

void unloadChessPieces(struct ChessPieces pieces) {
    printf("\n=== Unloading Chess Pieces ===\n");

    // Unload single pieces
    if (pieces.whiteKing.id) {
        printf("Unloading White King...\n");
        UnloadTexture(pieces.whiteKing);
    }
    if (pieces.blackKing.id) {
        printf("Unloading Black King...\n");
        UnloadTexture(pieces.blackKing);
    }
    if (pieces.whiteQueen.id) {
        printf("Unloading White Queen...\n");
        UnloadTexture(pieces.whiteQueen);
    }
    if (pieces.blackQueen.id) {
        printf("Unloading Black Queen...\n");
        UnloadTexture(pieces.blackQueen);
    }

    // Unload paired pieces
    for (int i = 0; i < 2; i++) {
        printf("\nUnloading set %d of paired pieces...\n", i + 1);
        if (pieces.whiteRook[i].id) UnloadTexture(pieces.whiteRook[i]);
        if (pieces.blackRook[i].id) UnloadTexture(pieces.blackRook[i]);
        if (pieces.whiteKnight[i].id) UnloadTexture(pieces.whiteKnight[i]);
        if (pieces.blackKnight[i].id) UnloadTexture(pieces.blackKnight[i]);
        if (pieces.whiteBishop[i].id) UnloadTexture(pieces.whiteBishop[i]);
        if (pieces.blackBishop[i].id) UnloadTexture(pieces.blackBishop[i]);
    }

    // Unload pawns
    for (int i = 0; i < 8; i++) {
        if (pieces.whitePawn[i].id) UnloadTexture(pieces.whitePawn[i]);
        if (pieces.blackPawn[i].id) UnloadTexture(pieces.blackPawn[i]);
    }

    printf("=== All Chess Pieces Unloaded ===\n\n");
}

