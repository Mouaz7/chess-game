#pragma once
#include "Position.h"

class Move {
private:
    Position<int> from;
    Position<int> to;
    bool isCapture;
    bool isCastling;

public:
    Move(Position<int> f, Position<int> t, bool capture = false, bool castling = false)
        : from(f), to(t), isCapture(capture), isCastling(castling) {}

    Position<int> getFrom() const { return from; }
    Position<int> getTo() const { return to; }
    bool getIsCapture() const { return isCapture; }
    bool getIsCastling() const { return isCastling; }
};
