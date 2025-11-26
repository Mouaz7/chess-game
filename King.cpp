#include "King.h"
#include <cmath>

King::King(PieceColor c, Position<int> pos) : Piece(c, pos) {}

PieceType King::getType() const {
    return PieceType::KING;
}

char King::getSymbol() const {
    return (color == PieceColor::WHITE) ? 'K' : 'k';
}

bool King::isValidMove(Position<int> to, Piece* board[8][8]) const {
    if (!to.isValid()) return false;
    
    int rowDiff = std::abs(to.getRow() - position.getRow());
    int colDiff = std::abs(to.getCol() - position.getCol());

    // King can move one square in any direction
    if (rowDiff <= 1 && colDiff <= 1 && (rowDiff + colDiff) > 0) {
        Piece* targetPiece = board[to.getRow()][to.getCol()];
        return targetPiece == nullptr || targetPiece->getColor() != color;
    }

    // Castling: King moves 2 squares horizontally
    if (rowDiff == 0 && colDiff == 2 && !hasMoved) {
        int row = position.getRow();
        int kingCol = position.getCol();
        int targetCol = to.getCol();
        
        // Determine if kingside or queenside castling
        bool isKingside = targetCol > kingCol;
        int rookCol = isKingside ? 7 : 0;
        
        // Check if rook exists and hasn't moved
        Piece* rook = board[row][rookCol];
        if (rook == nullptr || rook->getType() != PieceType::ROOK || 
            rook->getColor() != color || rook->getHasMoved()) {
            return false;
        }
        
        // Check if path between king and rook is clear
        int step = isKingside ? 1 : -1;
        for (int col = kingCol + step; col != rookCol; col += step) {
            if (board[row][col] != nullptr) {
                return false;
            }
        }
        
        // Note: We cannot check here if king is in check or passes through check
        // because that would require access to the Board's isSquareUnderAttack method.
        // This validation will be done in Board::movePiece() using wouldLeaveKingInCheck.
        // The king's starting square, passing square, and ending square will all be validated.
        
        return true;
    }

    return false;
}
