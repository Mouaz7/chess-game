#pragma once
#include "Piece.h"
#include <cmath>

class King : public Piece {
public:
    King(PieceColor c, Position<int> pos) : Piece(c, pos) {}

    PieceType getType() const override {
        return PieceType::KING;
    }

    char getSymbol() const override {
        return (color == PieceColor::WHITE) ? 'K' : 'k';
    }

    bool isValidMove(Position<int> to, Piece* board[8][8]) const override {
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
};

class Queen : public Piece {
public:
    Queen(PieceColor c, Position<int> pos) : Piece(c, pos) {}

    PieceType getType() const override {
        return PieceType::QUEEN;
    }

    char getSymbol() const override {
        return (color == PieceColor::WHITE) ? 'Q' : 'q';
    }

    bool isValidMove(Position<int> to, Piece* board[8][8]) const override {
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
};

class Rook : public Piece {
public:
    Rook(PieceColor c, Position<int> pos) : Piece(c, pos) {}

    PieceType getType() const override {
        return PieceType::ROOK;
    }

    char getSymbol() const override {
        return (color == PieceColor::WHITE) ? 'R' : 'r';
    }

    bool isValidMove(Position<int> to, Piece* board[8][8]) const override {
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
};

class Bishop : public Piece {
public:
    Bishop(PieceColor c, Position<int> pos) : Piece(c, pos) {}

    PieceType getType() const override {
        return PieceType::BISHOP;
    }

    char getSymbol() const override {
        return (color == PieceColor::WHITE) ? 'B' : 'b';
    }

    bool isValidMove(Position<int> to, Piece* board[8][8]) const override {
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
};

class Knight : public Piece {
public:
    Knight(PieceColor c, Position<int> pos) : Piece(c, pos) {}

    PieceType getType() const override {
        return PieceType::KNIGHT;
    }

    char getSymbol() const override {
        return (color == PieceColor::WHITE) ? 'N' : 'n';
    }

    bool isValidMove(Position<int> to, Piece* board[8][8]) const override {
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
};

class Pawn : public Piece {
public:
    Pawn(PieceColor c, Position<int> pos) : Piece(c, pos) {}

    PieceType getType() const override {
        return PieceType::PAWN;
    }

    char getSymbol() const override {
        return (color == PieceColor::WHITE) ? 'P' : 'p';
    }

    bool isValidMove(Position<int> to, Piece* board[8][8]) const override {
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
};
