# TODO - Future Improvements

## ?? Priority 1: Critical Features (for complete chess game)

### Checkmate Detection
- [ ] Implement function to check if king is in check
- [ ] Check if there are any valid moves left
- [ ] Display "Checkmate!" message
- [ ] Stop game after checkmate

```cpp
// Suggested implementation:
bool Board::isInCheck(PieceColor color) const {
    // Find king's position
    // Check if any opponent piece can capture the king
}

bool Board::isCheckmate(PieceColor color) const {
    if (!isInCheck(color)) return false;
    // Check if there is any move that can avoid check
}
```

### Check Warning
- [ ] Show red highlight around king when in check
- [ ] Sound effect when check occurs
- [ ] Prevent moves that put own king in check

---

## ?? Priority 2: Chess Rules

### En Passant
- [ ] Track when a pawn makes double-step
- [ ] Allow en passant next move
- [ ] Remove captured pawn correctly

```cpp
// Add to Pawn class:
bool canEnPassant(Position<int> to, Piece* board[8][8], Move lastMove) const;
```

### Castling (complete)
- [ ] Check that neither king nor rook has moved
- [ ] Check that path is clear
- [ ] Check that king doesn't pass through check
- [ ] Move both king and rook

```cpp
// Add to King class:
bool canCastle(Position<int> rookPos, Piece* board[8][8]) const;
void performCastling(Position<int> rookPos, Board& board);
```

### Pawn Promotion
- [ ] Detect when pawn reaches last row
- [ ] Show dialog to choose new piece (Queen, Rook, Bishop, Knight)
- [ ] Replace pawn with chosen piece

```cpp
// Add to Board::movePiece:
if (piece->getType() == PieceType::PAWN && isLastRow(to)) {
    promotePawn(to);
}
```

---

## ?? Priority 3: Graphics

### Improve Graphics
- [ ] Load real chess piece sprites (PNG/SVG)
- [ ] Add coordinates (a-h, 1-8) on board
- [ ] Better color scheme (customizable)
- [ ] Animations for moves
- [ ] Smooth transitions

```cpp
// Load sprites:
textures[0].loadFromFile("assets/white_king.png");
textures[1].loadFromFile("assets/white_queen.png");
// ...
```

### Font for Text
- [ ] Include a font file (e.g. Arial.ttf)
- [ ] Display piece symbols with font
- [ ] Add text for player info
- [ ] Show move history on screen

---

## ?? Priority 4: AI Opponent

### Implement Minimax Algorithm
- [ ] Basic Minimax (depth 3-4)
- [ ] Alpha-Beta pruning for optimization
- [ ] Position evaluation (material, position, mobility)
- [ ] Opening book for varied games

```cpp
class ChessAI {
public:
    Move getBestMove(Board& board, int depth);
private:
    int minimax(Board& board, int depth, bool maximizing);
    int evaluatePosition(Board& board);
};
```

### Difficulty Levels
- [ ] Easy (depth 2, quick moves)
- [ ] Medium (depth 4, better evaluation)
- [ ] Hard (depth 6, advanced evaluation)

---

## ?? Priority 5: User Experience

### Game Features
- [ ] Undo/Redo (undo moves)
- [ ] Save/Load game (not just history)
- [ ] Game timer (chess clock)
- [ ] Hint system (suggest moves)
- [ ] Show possible moves for selected piece

### UI Improvements
- [ ] Main menu (New Game, Load, Settings, Quit)
- [ ] Settings (colors, sound, difficulty)
- [ ] Player names
- [ ] Scoring system
- [ ] Statistics (wins, losses, moves)

---

## ?? Priority 6: Sound

### Sound Effects
- [ ] Move sound
- [ ] Capture sound
- [ ] Check warning
- [ ] Checkmate sound
- [ ] Button click

```cpp
sf::SoundBuffer moveBuffer;
moveBuffer.loadFromFile("assets/move.wav");
sf::Sound moveSound(moveBuffer);
```

---

## ?? Priority 7: Network Play

### Multiplayer
- [ ] Server-client architecture
- [ ] Matchmaking
- [ ] Chat functionality
- [ ] Ranking system
- [ ] Replay system

---

## ?? Priority 8: Documentation

### Code Documentation
- [ ] Doxygen comments for all classes
- [ ] Generate HTML documentation
- [ ] UML diagrams with tools (PlantUML)
- [ ] Architecture diagrams

### User Manual
- [ ] Tutorial for beginners
- [ ] Video guide
- [ ] FAQ
- [ ] Troubleshooting guide

---

## ?? Priority 9: Testing

### Unit Tests
- [ ] Google Test or Catch2
- [ ] Test all piece moves
- [ ] Test checkmate situations
- [ ] Test file I/O

```cpp
TEST(PawnTest, ValidMoveForward) {
    Board board;
    Pawn pawn(PieceColor::WHITE, Position<int>(6, 4));
    EXPECT_TRUE(pawn.isValidMove(Position<int>(5, 4), board.getBoard()));
}
```

### Integration Tests
- [ ] Test complete game flow
- [ ] Test AI opponent
- [ ] Test network play

---

## ?? Priority 10: Distribution

### Packaging
- [ ] Windows installer (NSIS or WiX)
- [ ] Include all DLLs
- [ ] Desktop shortcut
- [ ] Auto-update function

### Cross-platform
- [ ] Linux build (AppImage)
- [ ] macOS build (.dmg)
- [ ] CMake configuration for all platforms

---

## ?? Known Bugs (to fix)

### Critical
- [ ] No checkmate detection (can play after game is over)
- [ ] No check validation (can put own king in check)

### Minor
- [ ] Font missing (symbols don't display)
- [ ] No piece images (uses circles)
- [ ] En passant not implemented
- [ ] Castling not complete

### Nice-to-fix
- [ ] Better color choices for board
- [ ] Animations for moves
- [ ] Highlight for last move

---

## ?? Ideas for the Future

### Game Modes
- [ ] Chess960 (Fischer Random)
- [ ] Bughouse Chess (4 players)
- [ ] King of the Hill
- [ ] Three-check
- [ ] Atomic Chess

### Analysis Tools
- [ ] Move analysis with stockfish
- [ ] Opening explorer
- [ ] Endgame tablebase
- [ ] Real-time position evaluation

### Social Features
- [ ] Friends system
- [ ] Tournament system
- [ ] Achievements/Trophies
- [ ] Replays and sharing

---

## ?? Estimated Timeline

If project continues:

- **Week 1-2**: Checkmate detection and check warning
- **Week 3-4**: Improve graphics and UI
- **Week 5-6**: Implement AI (basic)
- **Week 7-8**: Testing and bugfixes
- **Week 9-10**: Network play (if time permits)

---

## ?? Final Goal

A professional chess game with:
- ? All chess rules correctly implemented
- ? Beautiful graphics and animations
- ? Smart AI opponent
- ? Network play
- ? Complete user experience

---

**Status**: This is a TODO list for future development.  
**Current version**: Meets all requirements for the course.  
**Next step**: Pick an item from Priority 1 and start implementing!
