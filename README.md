# Schack – C++ SFML Chess Game

A lightweight chess application built with **C++** and **SFML 3.0**, inspired by Chess.com’s visual style.

## Features

- Classic green board theme (Chess.com colors)
- Move hints: dots for empty squares, rings for capture squares
- Material score display (+N for the player with material advantage)
- Player name input at startup
- 10‑minute per‑player timer with active‑turn highlighting
- Automatic match‑result logging to `match_results.txt`
- Clean, modular code (Board, Game, Pieces, Move validation)

## Getting Started

### Prerequisites

- Windows 10/11
- Visual Studio 2022 (C++ tools)
- SFML 3.0 (installed via vcpkg)

### Build & Run

```powershell
cd C:\Users\mouaz\source\repos\Schack\Schack
powershell -ExecutionPolicy Bypass -File .\build_and_run.ps1
```

The script finds MSBuild, compiles the project, and launches the game.

## Play

1. Enter the white and black player names in the console.
2. Click a piece – valid moves appear as gray dots/rings.
3. Move by clicking a highlighted square. The timer and material advantage update in real time.

## Project Structure

```
/src
   main.cpp          – entry point
   Game.h/.cpp       – game loop, UI, timers
   Board.h/.cpp      – board representation, move generation, rendering
   Pieces.h/.cpp     – piece classes and movement rules
/assets
   *.png             – high‑resolution piece images
```

## Contributing

1. Fork the repository.
2. Create a feature branch (`git checkout -b feature/your‑idea`).
3. Implement your changes, run the build script to verify, then submit a Pull Request.

## License

MIT – you may use, modify, and distribute this code.
