#include "Piece.h"

Piece::Piece(PieceColor c, Position<int> pos) 
    : color(c), position(pos), hasMoved(false), texture(nullptr) {}

Piece::~Piece() {}

PieceColor Piece::getColor() const { 
    return color; 
}

Position<int> Piece::getPosition() const { 
    return position; 
}

void Piece::setPosition(Position<int> pos) { 
    position = pos; 
}

bool Piece::getHasMoved() const { 
    return hasMoved; 
}

void Piece::setHasMoved(bool moved) { 
    hasMoved = moved; 
}

void Piece::setTexture(sf::Texture* tex) { 
    texture = tex; 
}

sf::Texture* Piece::getTexture() const { 
    return texture; 
}

bool Piece::isPathClear(Position<int> to, Piece* board[8][8]) const {
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
