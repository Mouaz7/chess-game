#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "Board.h"
#include "GameHistory.h"
#include "Position.h"

class Game {
private:
    std::unique_ptr<Board> board;
    std::unique_ptr<GameHistory> history;
    sf::RenderWindow window;
    Position<int> selectedSquare;
    bool isPieceSelected;
    std::vector<Position<int>> currentValidMoves;
    
    // Player information
    std::string whitePlayerName;
    std::string blackPlayerName;
    
    // Timer
    sf::Clock gameClock;
    sf::Clock turnClock;
    int whiteTimeRemaining;  // in seconds
    int blackTimeRemaining;  // in seconds
    bool timerEnabled;
    
    // Game state
    bool gameOver;
    bool isCheck;
    std::string gameResult;
    std::time_t gameStartTime;
    
    // Pawn promotion
    bool waitingForPromotion;
    Position<int> promotionSquare;
    PieceColor promotionColor;
    
    // UI font
    sf::Font uiFont;
    bool uiFontLoaded;

    // View for scaling
    sf::View gameView;
    const float LOGICAL_WIDTH = BOARD_SIZE * SQUARE_SIZE + 2 * BORDER_SIZE + 200;
    const float LOGICAL_HEIGHT = BOARD_SIZE * SQUARE_SIZE + 2 * BORDER_SIZE;

    // Private helper methods
    Position<int> getSquareFromMouse(int mouseX, int mouseY);
    void updateTimer();
    std::string formatTime(int seconds);
    void saveMatchResult(const std::string& resultType);
    void handleEvents();
    void handleMouseClick(int mouseX, int mouseY);
    void render();

public:
    Game();
    ~Game();
    void run();
};
