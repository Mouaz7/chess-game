# Quick Start Guide - Chess Game

## Step 1: Install SFML

### Option A: Via NuGet (Easiest - Recommended)
1. Open `Schack.sln` in Visual Studio
2. Go to Tools ? NuGet Package Manager ? Manage NuGet Packages for Solution
3. Search for "SFML"
4. Install the following packages:
   - `sfml-system`
   - `sfml-window`
   - `sfml-graphics`

### Option B: Run installation script
```powershell
cd Schack
.\install_sfml.ps1
```

### Option C: Via vcpkg
```powershell
vcpkg install sfml:x64-windows
vcpkg integrate install
```

## Step 2: Build the project
1. Open `Schack.sln` in Visual Studio
2. Select **x64** and **Debug** or **Release**
3. Press `Ctrl + Shift + B` to build
4. If you get errors, see [INSTALLATION.md](INSTALLATION.md)

## Step 3: Run the game
1. Press `F5` or `Ctrl + F5`
2. A window with the chessboard should appear

## Controls
- **Left mouse click**: Select piece ? Click again to move
- **S**: Save game history to file
- **R**: Reset game
- **ESC/Close window**: Exit (saves automatically)

## Game Rules
- White starts
- Turn order alternates automatically
- Invalid moves are rejected
- All chess rules are implemented (except en passant and checkmate detection)

## Files Created
- `game_history.txt` - Game history (created automatically)

## Troubleshooting
See [INSTALLATION.md](INSTALLATION.md) for detailed troubleshooting.

## More Information
- [README.md](README.md) - Complete project description
- [INSTALLATION.md](INSTALLATION.md) - Installation guide
