#include "Queen.h"
#include <cmath>

Queen::Queen(PieceColor c, Position<int> pos) : Piece(c, pos) {}

PieceType Queen::getType() const {
    return PieceType::QUEEN;
}

char Queen::getSymbol() const {
    return (color == PieceColor::WHITE) ? 'Q' : 'q';
}

bool Queen::isValidMove(Position<int> to, Piece* board[8][8]) const {
    if (!to.isValid()) return false;
    if (to == position) return false;
    
    int rowDiff = std::abs(to.getRow() - position.getRow());
    int colDiff = std::abs(to.getCol() - position.getCol());

    // Queen moves like rook or bishop
    bool isDiagonal = (rowDiff == colDiff && rowDiff > 0);
    bool isStraight = (rowDiff == 0 && colDiff > 0) || (colDiff == 0 && rowDiff > 0);

    if (isDiagonal || isStraight) {
        if (!isPathClear(to, board)) return false;
        
        Piece* targetPiece = board[to.getRow()][to.getCol()];
        return targetPiece == nullptr || targetPiece->getColor() != color;
    }
    return false;
}
