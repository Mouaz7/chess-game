#include "Knight.h"
#include <cmath>

Knight::Knight(PieceColor c, Position<int> pos) : Piece(c, pos) {}

PieceType Knight::getType() const {
    return PieceType::KNIGHT;
}

char Knight::getSymbol() const {
    return (color == PieceColor::WHITE) ? 'N' : 'n';
}

bool Knight::isValidMove(Position<int> to, Piece* board[8][8]) const {
    if (!to.isValid()) return false;
    if (to == position) return false;
    
    int rowDiff = std::abs(to.getRow() - position.getRow());
    int colDiff = std::abs(to.getCol() - position.getCol());

    // Knight moves in L-shape
    bool isLShape = (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);

    if (isLShape) {
        Piece* targetPiece = board[to.getRow()][to.getCol()];
        return targetPiece == nullptr || targetPiece->getColor() != color;
    }
    return false;
}
