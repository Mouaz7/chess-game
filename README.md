# Schack – C++ SFML Chess Game

A lightweight chess application built with **C++** and **SFML 3.0**, inspired by Chess.com's visual style.

![](https://img.shields.io/badge/C%2B%2B-20-blue) ![](https://img.shields.io/badge/SFML-3.0-green) ![](https://img.shields.io/badge/License-MIT-yellow)

## Features

- **Chess.com-inspired UI**: Classic green board theme with professional aesthetics
- **Move hints**: Visual indicators showing legal moves (dots for empty squares, rings for captures)
- **Material advantage display**: Real-time material score showing which player is ahead
- **Player customization**: Enter custom player names at startup
- **Game timer**: 10-minute per-player chess clock with active-turn highlighting
- **Move history**: Automatic logging of all moves to `game_history.txt`
- **Match results**: Game outcomes saved to `match_results.txt`
- **Clean architecture**: Modular code with separate Board, Game, Pieces, and Move validation

## Technology Stack

### Programming Language

- **C++20**: Modern C++ with features like smart pointers, templates, lambda expressions, and RAII (Resource Acquisition Is Initialization)
- Provides high performance, type safety, and automatic memory management
- No manual memory allocation/deallocation needed

### Graphics Library

- **SFML 3.0** (Simple and Fast Multimedia Library)
  - Cross-platform C++ library for graphics, window management, and user input
  - Handles rendering the chessboard, pieces, and UI elements
  - Provides easy-to-use graphics primitives and image loading
  - Built-in support for shapes, textures, sprites, and text rendering

### Build System & Tools

- **Visual Studio 2022**: Primary IDE and compiler (MSVC v143)
  - Integrated debugger and profiler
  - IntelliSense for code completion
  - Project management with .vcxproj files
- **MSBuild**: Microsoft's build engine for compiling C++ projects
- **vcpkg**: Modern C++ package manager for SFML and its dependencies
  - Manages library installation and integration
  - Ensures consistent builds across machines

### Dependencies (Managed by vcpkg)

All dependencies are automatically installed via vcpkg:

- **SFML** - Main graphics and multimedia framework
- **FreeType** - Font rendering engine (used by SFML for text)
- **FLAC, Vorbis, Ogg** - Audio codec libraries (used by SFML for sound)
- **libpng** - PNG image format support for piece graphics
- **zlib, brotli** - Compression libraries for file formats

## Prerequisites

Before you begin, ensure you have the following installed:

1. **Windows 10/11** operating system
2. **Git** – [Download from git-scm.com](https://git-scm.com/downloads)
3. **Visual Studio 2022** (Community/Professional/Enterprise)
   - Install the **Desktop development with C++** workload
   - Ensure **MSVC v143 – VS 2022 C++ x64/x86 build tools** is selected
   - Ensure **Windows 10 SDK** is selected
4. **vcpkg** – C++ package manager for installing SFML

### Installing vcpkg

Open PowerShell and run:

```powershell
# Clone vcpkg to your user profile
git clone https://github.com/microsoft/vcpkg.git $env:USERPROFILE\vcpkg

# Navigate to vcpkg directory
cd $env:USERPROFILE\vcpkg

# Bootstrap vcpkg
.\bootstrap-vcpkg.bat

# Integrate with Visual Studio (run this as Administrator if needed)
.\vcpkg integrate install
```

### Installing SFML 3.0

After installing vcpkg, install SFML:

```powershell
# Set environment variables
$env:VCPKG_ROOT = "$env:USERPROFILE\vcpkg"
$env:VCPKG_DEFAULT_TRIPLET = "x64-windows"

# Install SFML for x64-windows
.\vcpkg install sfml:x64-windows
```

> **Note**: SFML installation may take 5-10 minutes and requires ~120 MB of disk space.

## Getting the Source Code

The repository is lightweight (~6 MB). Dependencies are installed separately via vcpkg.

```powershell
git clone https://github.com/Mouaz7/chess-game.git
cd chess-game
```

After cloning, vcpkg will automatically restore dependencies when you first build the project.

## Understanding the Project Structure

### What's in the Repository (Tracked in Git)

```
chess-game/
├── *.h, *.cpp          # Source code files
├── assets/             # Piece images (PNG files)
├── vcpkg.json          # Dependency manifest
├── *.vcxproj           # Visual Studio project files
├── build_and_run.ps1   # Build automation script
├── .gitignore          # Git ignore rules
└── README.md           # This file
```

### What Gets Generated Locally (NOT in Git)

These folders/files are created when you build or run the game:

- **`vcpkg_installed/`** - SFML and dependencies (~120 MB, regenerated from vcpkg.json)
- **`x64/Debug/`** and **`x64/Release/`** - Compiled executables and build artifacts
- **`game_history.txt`** - Your game moves (created when you play)
- **`match_results.txt`** - Game outcomes (created when you play)

> **Important**: Never commit `vcpkg_installed/` or `x64/` to Git - they're automatically regenerated.

## Building and Running

You can build and run the chess game using **Visual Studio 2022**, **Visual Studio Code**, or **PowerShell scripts**.

### Option 1: Using Visual Studio 2022

1. **Open the solution**:

   - Launch Visual Studio 2022
   - Go to **File → Open → Project/Solution**
   - Navigate to the project folder and open `Schack.sln`

2. **Configure the build**:

   - In the top toolbar, set the configuration to **Release**
   - Set the platform to **x64**

3. **Build the project**:

   - Press **Ctrl+Shift+B** or go to **Build → Build Solution**
   - Wait for the build to complete (check the Output window for any errors)

4. **Run the game**:
   - Press **Ctrl+F5** (Start Without Debugging) or **F5** (Start Debugging)
   - The console will prompt you to enter player names
   - After entering names, the game window will appear

### Option 2: Using Visual Studio Code

1. **Open the project folder**:

   ```powershell
   # Navigate to your project directory
   cd <your-project-directory>

   # Open in VS Code
   code .
   ```

2. **Create VS Code configuration files**:

   Create a `.vscode` folder in the project root if it doesn't exist, then create the following files:

   **`.vscode/tasks.json`** (for building):

   ```json
   {
     "version": "2.0.0",
     "tasks": [
       {
         "label": "Build Schack",
         "type": "shell",
         "command": "powershell",
         "args": [
           "-ExecutionPolicy",
           "Bypass",
           "-File",
           "${workspaceFolder}\\build_and_run.ps1"
         ],
         "group": {
           "kind": "build",
           "isDefault": true
         },
         "problemMatcher": []
       }
     ]
   }
   ```

   **`.vscode/launch.json`** (for debugging):

   ```json
   {
     "version": "0.2.0",
     "configurations": [
       {
         "name": "Run Schack",
         "type": "cppvsdbg",
         "request": "launch",
         "program": "${workspaceFolder}\\x64\\Release\\Schack.exe",
         "args": [],
         "stopAtEntry": false,
         "cwd": "${workspaceFolder}",
         "environment": [],
         "console": "externalTerminal"
       }
     ]
   }
   ```

3. **Build and run**:
   - Press **Ctrl+Shift+B** to build the project
   - Press **F5** to run the game
   - Alternatively, use the **Run** panel on the left sidebar

### Option 3: Using PowerShell Scripts

The project includes helper scripts for quick building and running:

1. **Open PowerShell** in the project directory

2. **Allow script execution** (if needed):

   ```powershell
   Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
   ```

3. **Run the build script**:
   ```powershell
   .\build_and_run.ps1
   ```

The script will:

- Automatically locate MSBuild
- Build the project in Release mode
- Launch the executable

## How to Play

1. **Start the game**: Run the executable using any of the methods above
2. **Enter player names**: Type the names for White and Black players in the console
3. **Select a piece**: Click on any piece to see its valid moves (shown as gray dots/rings)
4. **Make a move**: Click on a highlighted square to move the selected piece
5. **Watch the timer**: Each player has 10 minutes; the active player's timer counts down
6. **Track material**: The sidebar shows which player has a material advantage

### Keyboard Controls

- **S**: Save game history to `game_history.txt`
- **R**: Reset the game
- **ESC**: Exit the game (automatically saves match results)

## Project Architecture

The game uses modern C++ design patterns and principles:

```
Core Components:
├── main.cpp              # Entry point and initialization
├── Game.h                # Game loop, UI rendering, timers, and event handling
├── Board.h               # Board state, move generation, and validation
├── Pieces.h              # Piece implementations (King, Queen, Rook, Bishop, Knight, Pawn)
├── Piece.h               # Abstract base class defining piece interface
├── Position.h            # Template class for board coordinates
├── Move.h                # Move representation and metadata
├── Enums.h               # PieceColor and PieceType enumerations
└── GameHistory.h         # Move history tracking and file I/O

Design Patterns Used:
- Factory Pattern: Creating pieces dynamically
- Strategy Pattern: Each piece has unique move validation logic
- RAII: Smart pointers for automatic memory management
- Polymorphism: Virtual functions for piece behavior
```

## Troubleshooting

### MSBuild not found

- **Solution**: Ensure Visual Studio 2022 is installed with the C++ workload. The `find_msbuild.ps1` script should automatically locate MSBuild.

### SFML headers missing

- **Solution**:
  - Verify that `vcpkg integrate install` was run successfully
  - Check that SFML was installed: `vcpkg list | Select-String sfml`
  - If needed, reinstall: `vcpkg install sfml:x64-windows`

### Permission denied when running scripts

- **Solution**:
  - Run PowerShell as Administrator
  - Or set execution policy: `Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass`

### "Not Responding" at startup

- **Solution**: This is normal. The window becomes responsive after you enter player names in the console.

### DLL not found errors

- **Solution**: Ensure you're building in x64 mode and that vcpkg integration is active. The SFML DLLs should be automatically copied by vcpkg.

### Build errors after cloning

- **Solution**:
  - Clean the solution: **Build → Clean Solution** in Visual Studio
  - Delete the `x64` folder if it exists
  - Rebuild: **Build → Rebuild Solution**

### vcpkg_installed folder is missing

- **Solution**: This is normal for a fresh clone. Run the build and vcpkg will automatically restore dependencies based on `vcpkg.json`.

## Contributing

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/your-idea`
3. Make your changes and test thoroughly
4. Run the build script to verify: `.\build_and_run.ps1`
5. Commit your changes: `git commit -m "Add your feature"`
6. Push to your fork: `git push origin feature/your-idea`
7. Submit a Pull Request

**Please ensure**:

- Your code follows C++20 standards
- No build warnings or errors
- Code is well-commented for complex logic

## License

MIT License – You may use, modify, and distribute this code freely.

---

**Enjoy playing chess!** 🎮♟️

Made with ❤️ using C++ and SFML
