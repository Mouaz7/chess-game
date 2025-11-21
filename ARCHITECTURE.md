# Architecture and Design

## Overview

This chess game is built using modern C++ with object-oriented design principles and SFML for graphics.

## Design Principles

### SOLID Principles

1. **Single Responsibility Principle**
   - Each class has one clear purpose
   - `Piece`: Represents a chess piece
   - `Board`: Manages the game board
   - `Game`: Handles game flow and UI
   - `GameHistory`: Manages move history

2. **Open/Closed Principle**
   - Easy to add new piece types by extending `Piece`
   - No need to modify existing code

3. **Liskov Substitution Principle**
   - All piece classes can be used wherever `Piece` is expected
   - Polymorphism works correctly

4. **Interface Segregation Principle**
   - Minimal, focused interfaces
   - Pure virtual functions define contract

5. **Dependency Inversion Principle**
   - Depends on `Piece` abstraction, not concrete pieces
   - Board works with any piece type

### Design Patterns

#### 1. Factory Pattern
Creating pieces in `Board::initializeBoard()`:
```cpp
pieces.push_back(std::make_unique<King>(color, position));
pieces.push_back(std::make_unique<Queen>(color, position));
// etc.
```

#### 2. Strategy Pattern
Each piece has its own move validation strategy:
```cpp
class Piece {
    virtual bool isValidMove(...) const = 0;
};

class King : public Piece {
    bool isValidMove(...) const override {
        // King-specific logic
    }
};
```

#### 3. Template Method Pattern
`Piece` defines the template for all pieces:
```cpp
class Piece {
protected:
    bool isPathClear(...) const; // Common implementation
public:
    virtual bool isValidMove(...) const = 0; // Must override
};
```

#### 4. RAII (Resource Acquisition Is Initialization)
Smart pointers manage memory automatically:
```cpp
std::vector<std::unique_ptr<Piece>> pieces;
// Automatically cleaned up when out of scope
```

## Class Hierarchy

```
Position<T> (Template class)
    - Generic coordinate representation
    - Can use any numeric type

Enums
    - PieceColor: WHITE, BLACK
    - PieceType: KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN

Move
    - Represents a single move
    - From position, to position
    - Capture flag, castling flag

Piece (Abstract base class)
    ??? King
    ??? Queen
    ??? Rook
    ??? Bishop
    ??? Knight
    ??? Pawn

Board
    - Contains: vector<unique_ptr<Piece>>
    - Manages: Piece* board[8][8]
    - Validates moves
    - Switches turns

GameHistory
    - Contains: vector<Move>
    - Saves to file
    - Loads from file
    - Exception handling

Game
    - Contains: unique_ptr<Board>
    - Contains: unique_ptr<GameHistory>
    - Manages: sf::RenderWindow
    - Handles events
    - Renders graphics
```

## Memory Management

### Smart Pointers
```cpp
// Ownership
std::unique_ptr<Piece> piece = std::make_unique<King>(...);

// Board owns pieces
std::vector<std::unique_ptr<Piece>> pieces;

// Raw pointers for references (no ownership)
Piece* board[8][8];
```

### Memory Safety
- No manual `new`/`delete`
- RAII ensures cleanup
- No memory leaks
- Exception safe

## Data Flow

### Move Execution
```
1. User clicks square
2. Game::handleMouseClick()
3. Board::getPieceAt() - Check if piece exists
4. User clicks destination
5. Piece::isValidMove() - Validate move (polymorphism)
6. Board::movePiece() - Execute if valid
7. Update internal state
8. GameHistory::addMove() - Record move
9. Switch turns
10. Render updated board
```

### Rendering
```
1. Game::run() - Main loop
2. Game::render()
3. Board::draw()
4. For each square:
   - Draw square color
   - If piece exists, draw piece
5. Highlight selected square
6. Display window
```

## File I/O

### Save Format
```
Chess Game History
==================

Move 1: (6,4) -> (4,4)
Move 2: (1,4) -> (3,4) [Capture]
...
```

### Error Handling
```cpp
try {
    saveToFile();
}
catch (const std::exception& e) {
    // Handle gracefully
    std::cerr << "Error: " << e.what() << "\n";
}
```

## Modern C++ Features

### Templates
```cpp
template<typename T>
class Position {
    T row, col;
};

Position<int> pos;     // For board coordinates
Position<float> fpos;  // Could use for other purposes
```

### Lambda Expressions
```cpp
int count = std::count_if(pieces.begin(), pieces.end(),
    [color](const std::unique_ptr<Piece>& piece) {
        return piece->getColor() == color;
    });
```

### Smart Pointers
```cpp
std::unique_ptr<Board> board;  // Exclusive ownership
// No shared_ptr needed - no shared ownership
```

### Move Semantics
```cpp
pieces.push_back(std::make_unique<King>(...));
// Moves ownership, no copying
```

## Performance Considerations

### Efficient Move Validation
```cpp
// Early returns for invalid moves
if (!to.isValid()) return false;
if (to == position) return false;

// Check path only if move pattern is valid
if (isDiagonal || isStraight) {
    if (!isPathClear(to, board)) return false;
}
```

### Memory Efficiency
- Smart pointers: minimal overhead
- No unnecessary copies
- Move semantics where possible
- const-correctness prevents accidental copies

### Time Complexity
- Move validation: O(1) to O(n) depending on piece
- Board update: O(n) where n = number of pieces
- Rendering: O(64) for squares + O(n) for pieces

## Testing Strategy

### Unit Testing (Planned)
```cpp
TEST(KingTest, ValidMove) {
    King king(PieceColor::WHITE, Position<int>(4, 4));
    EXPECT_TRUE(king.isValidMove(Position<int>(4, 5), board));
}
```

### Integration Testing (Planned)
- Full game flow
- Save/load functionality
- UI interactions

### Manual Testing (Current)
- Play through games
- Test each piece type
- Test edge cases

## Future Architecture Improvements

1. **Separation of Concerns**
   - Separate rendering from game logic
   - Create dedicated UI layer

2. **Event System**
   - Observer pattern for game events
   - Decouple game state from UI updates

3. **State Machine**
   - Manage game states (menu, playing, paused)
   - Clean state transitions

4. **AI Module**
   - Separate AI logic from game logic
   - Pluggable AI strategies

## Extensibility

### Adding New Piece Types
```cpp
class Archbishop : public Piece {
public:
    PieceType getType() const override {
        return PieceType::ARCHBISHOP;
    }
    
    bool isValidMove(...) const override {
        // Combine knight and bishop moves
        return knightMove || bishopMove;
    }
};
```

### Adding Game Modes
```cpp
class Chess960 : public Game {
    void initializeBoard() override {
        // Random starting position
    }
};
```

## Documentation

- Code comments for complex logic
- README for setup and usage
- This file for architecture
- TODO for future plans

## Conclusion

This architecture provides:
- ? Clean, maintainable code
- ? Easy to extend
- ? Type-safe with C++20
- ? Memory-safe with smart pointers
- ? Follows best practices and design patterns
