# Contributing to Chess Game with SFML

Thank you for your interest in contributing to this project! ??

## How to Contribute

### Reporting Bugs

If you find a bug, please create an issue with:
- Clear description of the bug
- Steps to reproduce
- Expected behavior
- Actual behavior
- Screenshots (if applicable)
- Your environment (OS, compiler version, SFML version)

### Suggesting Features

Feature requests are welcome! Please create an issue with:
- Clear description of the feature
- Why it would be useful
- How it should work
- Example use cases

### Code Contributions

1. **Fork the repository**
   ```bash
   git clone https://github.com/yourusername/chess-game-sfml.git
   cd chess-game-sfml
   ```

2. **Create a branch**
   ```bash
   git checkout -b feature/YourFeatureName
   ```

3. **Make your changes**
   - Follow the existing code style
   - Add comments for complex logic
   - Update documentation if needed

4. **Test your changes**
   - Build and run the project
   - Test all affected functionality
   - Make sure no existing features break

5. **Commit your changes**
   ```bash
   git add .
   git commit -m "Add: Description of your changes"
   ```

6. **Push to your fork**
   ```bash
   git push origin feature/YourFeatureName
   ```

7. **Create a Pull Request**
   - Go to the original repository
   - Click "New Pull Request"
   - Select your branch
   - Describe your changes

## Code Style

### C++ Guidelines
- Use C++20 features
- Follow modern C++ best practices
- Use smart pointers instead of raw pointers
- Use const-correctness
- Prefer RAII for resource management

### Naming Conventions
- Classes: `PascalCase` (e.g., `ChessBoard`, `GamePiece`)
- Functions: `camelCase` (e.g., `movePiece`, `isValidMove`)
- Variables: `camelCase` (e.g., `currentTurn`, `selectedSquare`)
- Constants: `UPPER_SNAKE_CASE` (e.g., `BOARD_SIZE`, `SQUARE_SIZE`)
- Private members: use `m_` prefix if preferred

### File Organization
```cpp
#pragma once  // Use instead of include guards

// System includes
#include <vector>
#include <memory>

// External library includes
#include <SFML/Graphics.hpp>

// Project includes
#include "YourHeader.h"

// Constants
const int CONSTANT_NAME = value;

// Class definition
class ClassName {
private:
    // Private members first
    
public:
    // Public interface
    
private:
    // Private methods
};
```

### Comments
```cpp
// Single line comments for brief explanations

/**
 * @brief Longer description for complex functions
 * @param paramName Description of parameter
 * @return Description of return value
 */
```

## What to Contribute

Check [TODO.md](TODO.md) for ideas! Prioritized areas:

1. **Critical Features**
   - Checkmate detection
   - Check validation
   - Complete chess rules

2. **Improvements**
   - Better graphics
   - Sound effects
   - AI opponent

3. **Code Quality**
   - Unit tests
   - Bug fixes
   - Performance optimizations

4. **Documentation**
   - Code comments
   - User guides
   - Examples

## Testing

Before submitting:

1. **Build test**
   ```bash
   # Make sure it compiles
   cmake --build . --config Release
   ```

2. **Functionality test**
   - Run the game
   - Test your changes
   - Test existing features still work

3. **Code review**
   - Check for memory leaks
   - Verify no undefined behavior
   - Ensure proper error handling

## Pull Request Guidelines

- Keep PRs focused on a single feature/fix
- Write clear commit messages
- Update documentation if needed
- Add comments for complex code
- Test thoroughly before submitting

### Commit Message Format
```
Type: Brief description

Longer explanation if needed:
- Bullet points for details
- What changed
- Why it changed
```

Types:
- `Add:` New feature
- `Fix:` Bug fix
- `Update:` Update existing feature
- `Refactor:` Code restructuring
- `Docs:` Documentation changes
- `Test:` Adding tests
- `Style:` Code formatting

## Questions?

Feel free to open an issue for:
- Questions about the codebase
- Implementation discussions
- Architecture decisions
- Anything else!

## Code of Conduct

Be respectful and constructive:
- Be kind and welcoming
- Respect different viewpoints
- Accept constructive criticism
- Focus on what's best for the project

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

Thank you for contributing! ??
