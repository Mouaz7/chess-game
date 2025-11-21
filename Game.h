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
    std::string gameResult;
    std::time_t gameStartTime;
    
    // UI font
    sf::Font uiFont;
    bool uiFontLoaded;

    Position<int> getSquareFromMouse(int mouseX, int mouseY) {
        int row = mouseY / SQUARE_SIZE;
        int col = mouseX / SQUARE_SIZE;
        return Position<int>(row, col);
    }
    
    void updateTimer() {
        if (!timerEnabled || gameOver) return;
        
        float elapsed = turnClock.getElapsedTime().asSeconds();
        if (elapsed >= 1.0f) {
            // Update time for current player
            if (board->getCurrentTurn() == PieceColor::WHITE) {
                whiteTimeRemaining--;
                if (whiteTimeRemaining <= 0) {
                    whiteTimeRemaining = 0;
                    gameOver = true;
                    gameResult = blackPlayerName + " wins on time!";
                    saveMatchResult("Timeout");
                }
            } else {
                blackTimeRemaining--;
                if (blackTimeRemaining <= 0) {
                    blackTimeRemaining = 0;
                    gameOver = true;
                    gameResult = whitePlayerName + " wins on time!";
                    saveMatchResult("Timeout");
                }
            }
            turnClock.restart();
        }
    }
    
    std::string formatTime(int seconds) {
        int minutes = seconds / 60;
        int secs = seconds % 60;
        std::ostringstream oss;
        oss << minutes << ":" << std::setfill('0') << std::setw(2) << secs;
        return oss.str();
    }
    
    void saveMatchResult(const std::string& resultType) {
        std::ofstream file("match_results.txt", std::ios::app);
        if (!file.is_open()) {
            std::cerr << "Could not open match_results.txt\n";
            return;
        }
        
        // Get current time
        std::time_t now = std::time(nullptr);
        std::tm timeInfo;
        localtime_s(&timeInfo, &now);
        char timeBuffer[100];
        std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M", &timeInfo);
        
        // Calculate game duration
        int duration = static_cast<int>(std::difftime(now, gameStartTime));
        int durationMin = duration / 60;
        int durationSec = duration % 60;
        
        // Determine winner
        std::string winner, loser;
        if (resultType == "Checkmate") {
            // The player who just moved won (current turn switched after last move)
            if (board->getCurrentTurn() == PieceColor::WHITE) {
                winner = blackPlayerName;
                loser = whitePlayerName;
            } else {
                winner = whitePlayerName;
                loser = blackPlayerName;
            }
        } else if (resultType == "Timeout") {
            if (whiteTimeRemaining <= 0) {
                winner = blackPlayerName;
                loser = whitePlayerName;
            } else {
                winner = whitePlayerName;
                loser = blackPlayerName;
            }
        } else if (resultType == "Stalemate") {
            winner = "Draw";
            loser = "Draw";
        }
        
        // Write result
        file << timeBuffer << " | "
             << whitePlayerName << " | "
             << blackPlayerName << " | "
             << winner << " | "
             << resultType << " | "
             << durationMin << ":" << std::setfill('0') << std::setw(2) << durationSec
             << "\n";
        
        file.close();
        std::cout << "\nMatch result saved to match_results.txt\n";
    }

public:
    Game() : 
        board(std::make_unique<Board>()),
        history(std::make_unique<GameHistory>()),
        selectedSquare(-1, -1),
        isPieceSelected(false),
        whiteTimeRemaining(600),  // 10 minutes = 600 seconds
        blackTimeRemaining(600),
        timerEnabled(true),
        gameOver(false),
        uiFontLoaded(false)
    {
        // Load UI font
        uiFontLoaded = uiFont.openFromFile("C:\\Windows\\Fonts\\arial.ttf");
        if (!uiFontLoaded) {
            std::cout << "Warning: Could not load UI font\n";
        }
        
        // Get player names
        std::cout << "\n=== CHESS GAME SETUP ===\n";
        std::cout << "Enter White player name: ";
        std::getline(std::cin, whitePlayerName);
        if (whitePlayerName.empty()) whitePlayerName = "White Player";
        
        std::cout << "Enter Black player name: ";
        std::getline(std::cin, blackPlayerName);
        if (blackPlayerName.empty()) blackPlayerName = "Black Player";
        
        std::cout << "\n" << whitePlayerName << " (White) vs " << blackPlayerName << " (Black)\n";
        std::cout << "Timer: 10 minutes per player\n\n";
        
        // Create window AFTER getting input to avoid "Not Responding" state
        window.create(sf::VideoMode({BOARD_SIZE * SQUARE_SIZE + 200, BOARD_SIZE * SQUARE_SIZE}), "Chess Game");
        window.setFramerateLimit(60);
        
        // Record game start time
        gameStartTime = std::time(nullptr);
        
        // Start the clocks
        gameClock.restart();
        turnClock.restart();
    }

    ~Game() {
        try {
            history->saveToFile();
            std::cout << "Game history saved!\n";
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }

    void run() {
        // Try to load previous history
        try {
            history->loadFromFile();
            std::cout << "Loaded " << history->getMoveCount() << " previous moves.\n";
        }
        catch (const std::exception&) {
            std::cout << "No previous history found. Starting new game.\n";
        }

        // Show piece information
        std::cout << "\nBoard initialized!\n";
        std::cout << "White pieces: " << board->countPieces(PieceColor::WHITE) << " pieces\n";
        std::cout << "Black pieces: " << board->countPieces(PieceColor::BLACK) << " pieces\n";
        std::cout << "Total: " << (board->countPieces(PieceColor::WHITE) + board->countPieces(PieceColor::BLACK)) << " pieces\n";
        std::cout << "\nWhite moves first! Click on a white piece to begin.\n\n";

        while (window.isOpen()) {
            updateTimer();
            handleEvents();
            render();
        }
    }

private:
    void handleEvents() {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePress->button == sf::Mouse::Button::Left) {
                    handleMouseClick(mousePress->position.x, mousePress->position.y);
                }
            }
            else if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPress->code == sf::Keyboard::Key::S) {
                    // Save history manually
                    try {
                        history->saveToFile();
                        std::cout << "History saved!\n";
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Error saving: " << e.what() << "\n";
                    }
                }
                else if (keyPress->code == sf::Keyboard::Key::R) {
                    // Reset game
                    board = std::make_unique<Board>();
                    history->clear();
                    isPieceSelected = false;
                    std::cout << "Game reset!\n";
                }
                else if (keyPress->code == sf::Keyboard::Key::Escape) {
                    window.close();
                }
            }
        }
    }

    void handleMouseClick(int mouseX, int mouseY) {
        Position<int> clickedSquare = getSquareFromMouse(mouseX, mouseY);

        if (!clickedSquare.isValid()) return;

        if (!isPieceSelected) {
            // Select a piece
            Piece* piece = board->getPieceAt(clickedSquare);
            if (piece != nullptr && piece->getColor() == board->getCurrentTurn()) {
                selectedSquare = clickedSquare;
                isPieceSelected = true;
                currentValidMoves = board->getValidMoves(selectedSquare);
                std::cout << "Selected piece at (" << clickedSquare.getRow() 
                         << "," << clickedSquare.getCol() << ")\n";
            }
        }
        else {
            // Try to move the piece
            if (selectedSquare == clickedSquare) {
                // Deselect
                isPieceSelected = false;
                currentValidMoves.clear();
                std::cout << "Piece deselected\n";
            }
            else {
                Piece* capturedPiece = board->getPieceAt(clickedSquare);
                bool isCapture = capturedPiece != nullptr && capturedPiece->getColor() != board->getCurrentTurn();

                if (board->movePiece(selectedSquare, clickedSquare)) {
                    std::cout << "Move made: (" << selectedSquare.getRow() 
                             << "," << selectedSquare.getCol() << ") -> ("
                             << clickedSquare.getRow() << "," << clickedSquare.getCol() << ")\n";
                    
                    // Add to history
                    history->addMove(Move(selectedSquare, clickedSquare, isCapture, false));
                    
                    PieceColor currentPlayer = board->getCurrentTurn();
                    
                    // Check for checkmate
                    if (board->isPlayerInCheckmate(currentPlayer)) {
                        gameOver = true;
                        std::string winner = (currentPlayer == PieceColor::WHITE ? blackPlayerName : whitePlayerName);
                        std::string loser = (currentPlayer == PieceColor::WHITE ? whitePlayerName : blackPlayerName);
                        gameResult = winner + " wins by checkmate!";
                        
                        std::cout << "\n*** CHECKMATE! ***\n";
                        std::cout << winner << " wins!\n";
                        std::cout << loser << " loses.\n\n";
                        
                        saveMatchResult("Checkmate");
                    }
                    // Check for stalemate
                    else if (board->isPlayerInStalemate(currentPlayer)) {
                        gameOver = true;
                        gameResult = "Game drawn by stalemate";
                        
                        std::cout << "\n*** STALEMATE! ***\n";
                        std::cout << "Game is a draw!\n\n";
                        
                        saveMatchResult("Stalemate");
                    }
                    // Check for check
                    else if (board->isPlayerInCheck(currentPlayer)) {
                        std::string playerName = (currentPlayer == PieceColor::WHITE ? whitePlayerName : blackPlayerName);
                        std::cout << playerName << " is in CHECK!\n";
                    }
                    
                    isPieceSelected = false;
                    currentValidMoves.clear();
                }
                else {
                    std::cout << "Invalid move!\n";
                }
            }
        }
    }

    void render() {
        window.clear(sf::Color(49, 46, 43));  // Chess.com dark background
        
        // Draw board and pieces with move hints
        board->draw(window, currentValidMoves);

        // Highlight selected square with Chess.com style (green highlight)
        if (isPieceSelected) {
            sf::RectangleShape highlight(sf::Vector2f(static_cast<float>(SQUARE_SIZE), static_cast<float>(SQUARE_SIZE)));
            highlight.setPosition(sf::Vector2f(static_cast<float>(selectedSquare.getCol() * SQUARE_SIZE), 
                                              static_cast<float>(selectedSquare.getRow() * SQUARE_SIZE)));
            // Chess.com style: Yellow-green highlight
            highlight.setFillColor(sf::Color(186, 202, 68, 150));  // Semi-transparent green
            highlight.setOutlineColor(sf::Color(186, 202, 68));
            highlight.setOutlineThickness(3);
            window.draw(highlight);
        }
        
        // Draw UI (player info and timer) on the right side
        if (uiFontLoaded) {
            float uiX = BOARD_SIZE * SQUARE_SIZE + 10;
            float uiY = 20;
            
            // Calculate material difference
            int whiteScore = board->getMaterialScore(PieceColor::WHITE);
            int blackScore = board->getMaterialScore(PieceColor::BLACK);
            int diff = whiteScore - blackScore;
            
            // Black player info (top)
            sf::Text blackNameText(uiFont);
            blackNameText.setString(blackPlayerName);
            blackNameText.setCharacterSize(20);
            blackNameText.setFillColor(sf::Color::White);
            blackNameText.setStyle(sf::Text::Bold);
            blackNameText.setPosition(sf::Vector2f(uiX, uiY));
            window.draw(blackNameText);
            
            // Black material score
            if (diff < 0) {
                sf::Text scoreText(uiFont);
                scoreText.setString("+" + std::to_string(-diff));
                scoreText.setCharacterSize(16);
                scoreText.setFillColor(sf::Color(180, 180, 180));
                scoreText.setPosition(sf::Vector2f(uiX + 130, uiY + 2));
                window.draw(scoreText);
            }
            
            // Black player timer
            sf::Text blackTimeText(uiFont);
            blackTimeText.setString(formatTime(blackTimeRemaining));
            blackTimeText.setCharacterSize(32);
            blackTimeText.setFillColor(board->getCurrentTurn() == PieceColor::BLACK ? 
                sf::Color(129, 182, 76) : sf::Color::White); // Chess.com green for active
            blackTimeText.setStyle(sf::Text::Bold);
            blackTimeText.setPosition(sf::Vector2f(uiX, uiY + 30));
            window.draw(blackTimeText);
            
            // Active turn indicator for black
            if (board->getCurrentTurn() == PieceColor::BLACK && !gameOver) {
                sf::RectangleShape activeBar(sf::Vector2f(180, 4));
                activeBar.setFillColor(sf::Color(129, 182, 76));
                activeBar.setPosition(sf::Vector2f(uiX, uiY + 70));
                window.draw(activeBar);
            }
            
            // Separator
            sf::RectangleShape separator(sf::Vector2f(180, 2));
            separator.setPosition(sf::Vector2f(uiX, BOARD_SIZE * SQUARE_SIZE / 2 - 20));
            separator.setFillColor(sf::Color(80, 80, 80));
            window.draw(separator);
            
            // White player info (bottom)
            float whiteY = BOARD_SIZE * SQUARE_SIZE - 100;
            
            // White player timer
            sf::Text whiteTimeText(uiFont);
            whiteTimeText.setString(formatTime(whiteTimeRemaining));
            whiteTimeText.setCharacterSize(32);
            whiteTimeText.setFillColor(board->getCurrentTurn() == PieceColor::WHITE ? 
                sf::Color(129, 182, 76) : sf::Color::White);
            whiteTimeText.setStyle(sf::Text::Bold);
            whiteTimeText.setPosition(sf::Vector2f(uiX, whiteY));
            window.draw(whiteTimeText);
            
            // Active turn indicator for white
            if (board->getCurrentTurn() == PieceColor::WHITE && !gameOver) {
                sf::RectangleShape activeBar(sf::Vector2f(180, 4));
                activeBar.setFillColor(sf::Color(129, 182, 76));
                activeBar.setPosition(sf::Vector2f(uiX, whiteY - 10));
                window.draw(activeBar);
            }
            
            // White player name
            sf::Text whiteNameText(uiFont);
            whiteNameText.setString(whitePlayerName);
            whiteNameText.setCharacterSize(20);
            whiteNameText.setFillColor(sf::Color::White);
            whiteNameText.setStyle(sf::Text::Bold);
            whiteNameText.setPosition(sf::Vector2f(uiX, whiteY + 40));
            window.draw(whiteNameText);
            
            // White material score
            if (diff > 0) {
                sf::Text scoreText(uiFont);
                scoreText.setString("+" + std::to_string(diff));
                scoreText.setCharacterSize(16);
                scoreText.setFillColor(sf::Color(180, 180, 180));
                scoreText.setPosition(sf::Vector2f(uiX + 130, whiteY + 42));
                window.draw(scoreText);
            }
            
            // Game result message
            if (gameOver && !gameResult.empty()) {
                sf::Text resultText(uiFont);
                resultText.setString("GAME OVER");
                resultText.setCharacterSize(24);
                resultText.setFillColor(sf::Color::Red);
                resultText.setStyle(sf::Text::Bold);
                resultText.setPosition(sf::Vector2f(uiX, BOARD_SIZE * SQUARE_SIZE / 2 + 20));
                window.draw(resultText);
                
                // Winner message
                sf::Text winnerText(uiFont);
                winnerText.setString(gameResult);
                winnerText.setCharacterSize(14);
                winnerText.setFillColor(sf::Color::Yellow);
                winnerText.setPosition(sf::Vector2f(uiX, BOARD_SIZE * SQUARE_SIZE / 2 + 55));
                window.draw(winnerText);
            }
        }

        window.display();
    }
};
