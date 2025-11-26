#pragma once
#include "Piece.h"

class Rook : public Piece {
public:
    Rook(PieceColor c, Position<int> pos);
    
    PieceType getType() const override;
    char getSymbol() const override;
    bool isValidMove(Position<int> to, Piece* board[8][8]) const override;
};
