# Schack – C++ SFML Chess Game

A lightweight chess application built with **C++** and **SFML 3.0**, inspired by Chess.com’s visual style.

## 📦 Prerequisites (from scratch)

1. **Git** – download and install from https://git-scm.com/downloads.
2. **Visual Studio 2022** (Community/Professional/Enterprise) with the **Desktop development with C++** workload.
   - Ensure the **MSVC v143 – VS 2022 C++ x64/x86 build tools** and **Windows 10 SDK** are selected.
3. **PowerShell** (comes with Windows) – you will run the provided scripts.
4. **vcpkg** – a C++ package manager.
   ```powershell
   git clone https://github.com/microsoft/vcpkg.git $env:USERPROFILE\vcpkg
   cd $env:USERPROFILE\vcpkg
   .\bootstrap-vcpkg.bat
   ```
5. **SFML 3.0** via vcpkg (installed globally for the project):
   ```powershell
   $env:VCPKG_ROOT = "$env:USERPROFILE\vcpkg"
   $env:VCPKG_DEFAULT_TRIPLET = "x64-windows"
   .\vcpkg install sfml:x64-windows
   ```
6. (Optional) **Visual Studio Code** – install from https://code.visualstudio.com/.
   - Install the **C/C++** extension by Microsoft.
   - Install the **CMake Tools** extension if you prefer CMake builds.

## 📂 Getting the source code

```powershell
# Clone the repository (replace with your fork if you plan to push changes)
git clone https://github.com/Mouaz7/chess-game.git
cd chess-game
```

## 🛠️ Building & running the project

The repository already contains two helper PowerShell scripts:

- `find_msbuild.ps1` – locates the correct `MSBuild.exe` on your machine.
- `build_and_run.ps1` – builds the solution in **Release|x64** and launches the executable.

### Step‑by‑step

1. **Open a PowerShell window** (run as Administrator if you encounter execution‑policy issues).
2. **Allow script execution** (you only need to do this once):
   ```powershell
   Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
   ```
3. **Run the build script**:
   ```powershell
   cd C:\Users\mouaz\source\repos\Schack\Schack
   powershell -ExecutionPolicy Bypass -File .\build_and_run.ps1
   ```
   - The script will call `find_msbuild.ps1` to locate MSBuild, compile the solution, and start `Schack.exe`.
   - If the build succeeds you will see the game window with the Chess.com‑style green board.

## 🖥️ Using Visual Studio 2022

1. Open `Schack.sln` (or the `Schack.vcxproj` file) via **File → Open → Project/Solution**.
2. Set the configuration to **Release** and platform to **x64** (top toolbar).
3. Build with **Ctrl+Shift+B** or **Build → Build Solution**.
4. Run with **Ctrl+F5** (start without debugging) to see the game.

## 🖥️ Using Visual Studio Code

1. Open the folder:
   ```powershell
   code C:\Users\mouaz\source\repos\Schack\Schack
   ```
2. Create a `.vscode` folder with two files:
   - **tasks.json** (build task):
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
           "group": "build",
           "problemMatcher": []
         }
       ]
     }
     ```
   - **launch.json** (run without debugging):
     ```json
     {
       "version": "0.2.0",
       "configurations": [
         {
           "name": "Run Schack",
           "type": "cppvsdbg",
           "request": "launch",
           "program": "${workspaceFolder}\\Release\\Schack.exe",
           "args": [],
           "stopAtEntry": false,
           "cwd": "${workspaceFolder}",
           "environment": [],
           "externalConsole": true
         }
       ]
     }
     ```
3. Press **Ctrl+Shift+B** to run the build task, then press **F5** to launch the game (or use the **Run** panel).

## 🧩 Troubleshooting

- **MSBuild not found** – ensure Visual Studio is installed with the C++ workload; the `find_msbuild.ps1` script will locate it automatically.
- **SFML headers missing** – verify that `vcpkg integrate install` was run, or add `C:\Users\<you>\vcpkg\installed\x64-windows\include` to the include directories and the corresponding lib path.
- **Permission denied when running scripts** – run PowerShell as Administrator or set the execution policy as shown above.
- **Runtime "Not Responding"** – make sure you run the script _after_ the console prompts for player names (the script already handles this).

## 📄 License

MIT – you may use, modify, and distribute this code.

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
