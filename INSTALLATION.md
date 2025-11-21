# Installation Guide for Chess Game

## Method 1: Installation via NuGet (Recommended)

This is the easiest method for Visual Studio.

### Step 1: Open the project
1. Open `Schack.sln` in Visual Studio 2022

### Step 2: Install SFML via NuGet
1. Right-click on "Schack" project in Solution Explorer
2. Select "Manage NuGet Packages..."
3. Click on "Browse" tab
4. Search for "SFML"
5. Install the following packages:
   - `sfml-system`
   - `sfml-window`
   - `sfml-graphics`
6. Click "Install"
7. Accept license agreement

### Step 3: Build the project
1. Press `Ctrl + Shift + B` or select Build ? Build Solution
2. If successful, press `F5` to run the program

---

## Method 2: Installation via vcpkg

### Step 1: Install vcpkg (if you don't have it)
```powershell
# Open PowerShell as administrator
cd C:\
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
```

### Step 2: Install SFML
```powershell
.\vcpkg install sfml:x64-windows
```

### Step 3: Configure Visual Studio
Visual Studio will automatically find SFML after `vcpkg integrate install`

### Step 4: Build the project
1. Open `Schack.sln`
2. Build project (`Ctrl + Shift + B`)

---

## Method 3: Manual SFML installation

### Step 1: Download SFML
1. Go to https://www.sfml-dev.org/download.php
2. Download "Visual C++ 17 (2022) - 64-bit"
3. Extract to `C:\SFML-2.6.0` (or other location)

### Step 2: Configure the project
1. Right-click on project ? Properties
2. Under C/C++ ? General ? Additional Include Directories:
   - Add: `C:\SFML-2.6.0\include`
3. Under Linker ? General ? Additional Library Directories:
   - Add: `C:\SFML-2.6.0\lib`
4. Under Linker ? Input ? Additional Dependencies:
   - Add: `sfml-graphics.lib;sfml-window.lib;sfml-system.lib`

### Step 3: Copy DLL files
Copy the following files from `C:\SFML-2.6.0\bin` to your project folder:
- `sfml-graphics-2.dll`
- `sfml-window-2.dll`
- `sfml-system-2.dll`

---

## Troubleshooting

### Problem: "Cannot open include file: 'SFML/Graphics.hpp'"
**Solution**: SFML is not correctly installed. Follow Method 1 or 2 again.

### Problem: "LNK2019: unresolved external symbol"
**Solution**: 
- Check that all SFML libraries are linked
- For Debug build, use `-d` suffix: `sfml-graphics-d.lib`

### Problem: "DLL not found" when running
**Solution**: Copy SFML DLL files to the same folder as the .exe file

### Problem: Program doesn't start
**Solution**: 
1. Check that you're running in x64 configuration
2. Check that SFML DLL files are in the correct folder
3. Run as administrator (right-click ? Run as administrator)

---

## Verification

To verify everything works:

1. Build project without errors
2. Run the program
3. A window with a chessboard should appear
4. You should be able to click on pieces and move them

---

## Alternative: Use vcpkg directly in project

Add `vcpkg.json` to project folder:

```json
{
  "name": "chess",
  "version": "1.0.0",
  "dependencies": [
    "sfml"
  ]
}
```

Then in PowerShell:
```powershell
vcpkg integrate project
```

---

## System Requirements

- **OS**: Windows 10/11
- **Compiler**: Visual Studio 2022 (v143)
- **C++ Standard**: C++20
- **SFML Version**: 2.5 or later

---

## Support

If you encounter problems:
1. Check that all steps are correctly followed
2. Check the Output window in Visual Studio for error messages
3. Verify that SFML versions match (Debug/Release)
