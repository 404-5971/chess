# Chess-Raylib Architecture

## Overview
Chess-Raylib is structured in layers:
1. GUI Layer (Raylib)
2. Game Logic Layer
3. Board State Management

## Component Breakdown

### GUI (gui.c)
- Handles window management
- Renders game board and pieces
- Processes user input
- Manages animations

### Game Logic (game_logic.c)
- Validates moves
- Manages game state
- Handles special moves
- Detects check/checkmate

### Piece Management (pieces.c)
- Loads piece textures
- Manages piece resources
- Handles piece rendering

## Data Flow
1. User Input → GUI Layer
2. GUI Layer → Game Logic
3. Game Logic → Board State
4. Board State → GUI Layer (render)

## File Structure
```
chess/
├── src/
│   ├── gui.c
│   ├── game_logic.c
│   └── pieces.c
├── header/
│   ├── gui.h
│   ├── game_logic.h
│   └── pieces.h
└── assets/
    └── images/
``` 