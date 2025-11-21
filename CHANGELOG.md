# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2025-01-21

### Added
- Complete chess game implementation with SFML
- All standard chess pieces (King, Queen, Rook, Bishop, Knight, Pawn)
- Move validation for all pieces
- Graphical chessboard with alternating colors
- Mouse interaction for selecting and moving pieces
- Visual highlighting of selected piece
- Automatic turn switching between white and black
- Game history saved to file (`game_history.txt`)
- Reset game functionality (R key)
- Save history manually (S key)
- Exit game (ESC key)

### Features
- Generic `Position<T>` template class for coordinates
- Abstract `Piece` base class with pure virtual functions
- Polymorphism and dynamic binding for piece movement
- Smart pointers (`unique_ptr`) for automatic memory management
- STL containers (`vector`, `string`)
- Exception handling for file operations
- Lambda expressions for filtering and counting
- Function pointers for move validation
- SFML graphics for visual display

### Technical
- C++20 standard
- SFML 2.5+ library
- Visual Studio 2022 project
- CMake support
- vcpkg integration
- NuGet package support

### Documentation
- Comprehensive README.md
- Installation guide (INSTALLATION.md)
- Quick start guide (QUICKSTART.md)
- Contributing guidelines (CONTRIBUTING.md)
- Code of Conduct (CODE_OF_CONDUCT.md)
- TODO list for future improvements
- MIT License

### Known Limitations
- No checkmate detection
- No check warnings
- Simple graphics (shapes instead of sprites)
- No piece images
- No font rendering for piece symbols
- No AI opponent
- No en passant
- Incomplete castling

## [Unreleased]

### Planned
- Checkmate detection
- Check validation
- Better graphics with piece sprites
- AI opponent (minimax algorithm)
- Complete castling
- En passant
- Pawn promotion
- Sound effects
- Game timer
- Undo/Redo functionality

---

## Version History

### Version Numbering
- **1.0.0**: Initial release with basic chess functionality
- **1.x.x**: Bug fixes and minor improvements
- **2.0.0**: Major features (AI, complete rules)
- **3.0.0**: Advanced features (network play, etc.)

---

[1.0.0]: https://github.com/yourusername/chess-game-sfml/releases/tag/v1.0.0
[Unreleased]: https://github.com/yourusername/chess-game-sfml/compare/v1.0.0...HEAD
