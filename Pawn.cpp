#include "Pawn.h"
#include <cmath>

Pawn::Pawn(PieceColor c, Position<int> pos) : Piece(c, pos) {}

PieceType Pawn::getType() const {
    return PieceType::PAWN;
}

char Pawn::getSymbol() const {
    return (color == PieceColor::WHITE) ? 'P' : 'p';
}

bool Pawn::isValidMove(Position<int> to, Piece* board[8][8]) const {
    if (!to.isValid()) return false;
    if (to == position) return false;
    
    int direction = (color == PieceColor::WHITE) ? -1 : 1;
    int rowDiff = to.getRow() - position.getRow();
    int colDiff = std::abs(to.getCol() - position.getCol());

    // Forward one square
    if (rowDiff == direction && colDiff == 0) {
        return board[to.getRow()][to.getCol()] == nullptr;
    }

    // Forward two squares from starting position
    if (!hasMoved && rowDiff == 2 * direction && colDiff == 0) {
        int middleRow = position.getRow() + direction;
        return board[middleRow][position.getCol()] == nullptr && 
               board[to.getRow()][to.getCol()] == nullptr;
    }

    // Diagonal capture (regular or en passant)
    if (rowDiff == direction && colDiff == 1) {
        Piece* targetPiece = board[to.getRow()][to.getCol()];
        
        // Regular diagonal capture
        if (targetPiece != nullptr && targetPiece->getColor() != color) {
            return true;
        }
        
        // En passant: diagonal to empty square
        // The opponent's pawn should be beside us (same row, different column)
        if (targetPiece == nullptr) {
            Piece* adjacentPiece = board[position.getRow()][to.getCol()];
            if (adjacentPiece != nullptr && 
                adjacentPiece->getType() == PieceType::PAWN &&
                adjacentPiece->getColor() != color) {
                // This could be en passant - Board::movePiece will validate last move
                return true;
            }
        }
    }

    return false;
}
