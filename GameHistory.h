#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include "Move.h"
#include "Enums.h"

class GameHistory {
private:
    std::vector<Move> moves;
    std::string filename;

public:
    GameHistory(const std::string& file = "game_history.txt");

    void addMove(const Move& move);
    void saveToFile();
    void loadFromFile();
    const std::vector<Move>& getMoves() const;
    void clear();
    size_t getMoveCount() const;
};
