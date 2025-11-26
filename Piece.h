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
    Piece(PieceColor c, Position<int> pos);
    virtual ~Piece();

    // Pure virtual functions - must be overridden
    virtual PieceType getType() const = 0;
    virtual bool isValidMove(Position<int> to, Piece* board[8][8]) const = 0;
    virtual char getSymbol() const = 0;

    // Accessor methods
    PieceColor getColor() const;
    Position<int> getPosition() const;
    void setPosition(Position<int> pos);
    bool getHasMoved() const;
    void setHasMoved(bool moved);
    void setTexture(sf::Texture* tex);
    sf::Texture* getTexture() const;

    // Helper function to check if path is clear
    bool isPathClear(Position<int> to, Piece* board[8][8]) const;
};
