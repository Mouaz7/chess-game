#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Position.h"
#include "Enums.h"

// Abstract base class for all chess pieces
class Piece {
protected:
    PieceColor color;
    Position<int> position;
    bool hasMoved;
    sf::Texture* texture;

public:
    Piece(PieceColor c, Position<int> pos) 
        : color(c), position(pos), hasMoved(false), texture(nullptr) {}
    
    virtual ~Piece() {}

    // Pure virtual function - must be overridden
    virtual PieceType getType() const = 0;
    
    // Pure virtual function - must be overridden
    virtual bool isValidMove(Position<int> to, Piece* board[8][8]) const = 0;
    
    // Virtual function with default implementation
    virtual char getSymbol() const = 0;

    PieceColor getColor() const { return color; }
    Position<int> getPosition() const { return position; }
    void setPosition(Position<int> pos) { position = pos; }
    bool getHasMoved() const { return hasMoved; }
    void setHasMoved(bool moved) { hasMoved = moved; }
    
    void setTexture(sf::Texture* tex) { texture = tex; }
    sf::Texture* getTexture() const { return texture; }

    // Helper function to check if path is clear
    bool isPathClear(Position<int> to, Piece* board[8][8]) const {
        int rowDir = (to.getRow() > position.getRow()) ? 1 : (to.getRow() < position.getRow()) ? -1 : 0;
        int colDir = (to.getCol() > position.getCol()) ? 1 : (to.getCol() < position.getCol()) ? -1 : 0;

        int currentRow = position.getRow() + rowDir;
        int currentCol = position.getCol() + colDir;

        while (currentRow != to.getRow() || currentCol != to.getCol()) {
            if (board[currentRow][currentCol] != nullptr) {
                return false;
            }
            currentRow += rowDir;
            currentCol += colDir;
        }
        return true;
    }
};
