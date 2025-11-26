#include "Bishop.h"
#include <cmath>

Bishop::Bishop(PieceColor c, Position<int> pos) : Piece(c, pos) {}

PieceType Bishop::getType() const {
    return PieceType::BISHOP;
}

char Bishop::getSymbol() const {
    return (color == PieceColor::WHITE) ? 'B' : 'b';
}

bool Bishop::isValidMove(Position<int> to, Piece* board[8][8]) const {
    if (!to.isValid()) return false;
    if (to == position) return false;
    
    int rowDiff = std::abs(to.getRow() - position.getRow());
    int colDiff = std::abs(to.getCol() - position.getCol());

    // Bishop moves diagonally
    if (rowDiff == colDiff && rowDiff > 0) {
        if (!isPathClear(to, board)) return false;
        
        Piece* targetPiece = board[to.getRow()][to.getCol()];
        return targetPiece == nullptr || targetPiece->getColor() != color;
    }
    return false;
}
