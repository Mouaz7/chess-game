#pragma once

// Generic template class for positions
template<typename T>
class Position {
private:
    T row;
    T col;

public:
    Position() : row(0), col(0) {}
    Position(T r, T c) : row(r), col(c) {}

    T getRow() const { return row; }
    T getCol() const { return col; }
    
    void setRow(T r) { row = r; }
    void setCol(T c) { col = c; }

    bool operator==(const Position<T>& other) const {
        return row == other.row && col == other.col;
    }

    bool operator!=(const Position<T>& other) const {
        return !(*this == other);
    }

    bool isValid() const {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }
};
