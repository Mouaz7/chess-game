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
    GameHistory(const std::string& file = "game_history.txt") 
        : filename(file) {}

    void addMove(const Move& move) {
        moves.push_back(move);
    }

    // Write to file with exception handling
    void saveToFile() {
        try {
            std::ofstream file(filename);
            if (!file.is_open()) {
                throw std::runtime_error("Could not open file for writing: " + filename);
            }

            file << "Chess Game History\n";
            file << "==================\n\n";

            int moveNumber = 1;
            for (const auto& move : moves) {
                file << "Move " << moveNumber++ << ": "
                     << "(" << move.getFrom().getRow() << "," << move.getFrom().getCol() << ") -> "
                     << "(" << move.getTo().getRow() << "," << move.getTo().getCol() << ")";
                
                if (move.getIsCapture()) {
                    file << " [Capture]";
                }
                if (move.getIsCastling()) {
                    file << " [Castling]";
                }
                file << "\n";
            }

            file.close();
        }
        catch (const std::exception& e) {
            throw std::runtime_error(std::string("Error saving history: ") + e.what());
        }
    }

    // Read from file with exception handling
    void loadFromFile() {
        try {
            std::ifstream file(filename);
            if (!file.is_open()) {
                throw std::runtime_error("Could not open file for reading: " + filename);
            }

            moves.clear();
            std::string line;
            
            // Skip header
            std::getline(file, line); // Title
            std::getline(file, line); // ===
            std::getline(file, line); // Empty line

            while (std::getline(file, line)) {
                if (line.empty()) continue;

                // Simple parsing of moves
                // Format: "Move X: (r1,c1) -> (r2,c2) [optional flags]"
                size_t fromStart = line.find('(');
                size_t fromEnd = line.find(')');
                size_t toStart = line.find('(', fromEnd);
                size_t toEnd = line.find(')', toStart);

                if (fromStart != std::string::npos && toStart != std::string::npos) {
                    std::string fromStr = line.substr(fromStart + 1, fromEnd - fromStart - 1);
                    std::string toStr = line.substr(toStart + 1, toEnd - toStart - 1);

                    int r1, c1, r2, c2;
                    char comma;
                    std::stringstream ss1(fromStr), ss2(toStr);
                    ss1 >> r1 >> comma >> c1;
                    ss2 >> r2 >> comma >> c2;

                    bool isCapture = line.find("[Capture]") != std::string::npos;
                    bool isCastling = line.find("[Castling]") != std::string::npos;

                    moves.push_back(Move(Position<int>(r1, c1), Position<int>(r2, c2), 
                                        isCapture, isCastling));
                }
            }

            file.close();
        }
        catch (const std::exception& e) {
            throw std::runtime_error(std::string("Error loading history: ") + e.what());
        }
    }

    const std::vector<Move>& getMoves() const {
        return moves;
    }

    void clear() {
        moves.clear();
    }

    size_t getMoveCount() const {
        return moves.size();
    }
};
