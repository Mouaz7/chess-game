#include "Rook.h"
#include <cmath>

Rook::Rook(PieceColor c, Position<int> pos) : Piece(c, pos) {}

PieceType Rook::getType() const {
    return PieceType::ROOK;
}

char Rook::getSymbol() const {
    return (color == PieceColor::WHITE) ? 'R' : 'r';
}

bool Rook::isValidMove(Position<int> to, Piece* board[8][8]) const {
    if (!to.isValid()) return false;
    if (to == position) return false;
    
    int rowDiff = std::abs(to.getRow() - position.getRow());
    int colDiff = std::abs(to.getCol() - position.getCol());

    // Rook moves straight
    bool isStraight = (rowDiff == 0 && colDiff > 0) || (colDiff == 0 && rowDiff > 0);

    if (isStraight) {
        if (!isPathClear(to, board)) return false;
        
        Piece* targetPiece = board[to.getRow()][to.getCol()];
        return targetPiece == nullptr || targetPiece->getColor() != color;
    }
    return false;
}
