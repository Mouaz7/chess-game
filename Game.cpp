#include "Game.h"

// Private helper methods
Position<int> Game::getSquareFromMouse(int mouseX, int mouseY) {
    // Convert window coordinates to view coordinates
    sf::Vector2f worldPos = window.mapPixelToCoords(sf::Vector2i(mouseX, mouseY), gameView);
    
    // Account for border
    float boardX = worldPos.x - BORDER_SIZE;
    float boardY = worldPos.y - BORDER_SIZE;
    
    // Check if click is within the board grid
    if (boardX < 0 || boardY < 0) return Position<int>(-1, -1);
    
    int row = static_cast<int>(boardY) / SQUARE_SIZE;
    int col = static_cast<int>(boardX) / SQUARE_SIZE;
    
    // Validate range
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        return Position<int>(-1, -1);
    }
    
    return Position<int>(row, col);
}

void Game::updateTimer() {
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

std::string Game::formatTime(int seconds) {
    int minutes = seconds / 60;
    int secs = seconds % 60;
    std::ostringstream oss;
    oss << minutes << ":" << std::setfill('0') << std::setw(2) << secs;
    return oss.str();
}

void Game::saveMatchResult(const std::string& resultType) {
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

void Game::handleEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
            // Handle window resizing with letterboxing
            float windowRatio = static_cast<float>(resized->size.x) / static_cast<float>(resized->size.y);
            float viewRatio = LOGICAL_WIDTH / LOGICAL_HEIGHT;
            float sizeX = 1;
            float sizeY = 1;
            float posX = 0;
            float posY = 0;

            if (windowRatio > viewRatio) {
                sizeX = viewRatio / windowRatio;
                posX = (1 - sizeX) / 2.f;
            }
            else {
                sizeY = windowRatio / viewRatio;
                posY = (1 - sizeY) / 2.f;
            }

            gameView.setViewport(sf::FloatRect({posX, posY}, {sizeX, sizeY}));
            window.setView(gameView);
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

void Game::handleMouseClick(int mouseX, int mouseY) {
    // Prevent interaction if game is over
    if (gameOver) {
        return;
    }
    
    // Handle pawn promotion selection first
    if (waitingForPromotion) {
        // Promotion UI is in sidebar (x > BOARD_SIZE * SQUARE_SIZE + 2 * BORDER_SIZE)
        sf::Vector2f worldPos = window.mapPixelToCoords(sf::Vector2i(mouseX, mouseY), gameView);
        if (worldPos.x > BOARD_SIZE * SQUARE_SIZE + 2 * BORDER_SIZE) {
            int buttonY = static_cast<int>(worldPos.y);
            PieceType selectedType = PieceType::QUEEN; // Default
            
            // Simple button layout: 4 buttons stacked vertically
            if (buttonY >= 200 && buttonY < 250) selectedType = PieceType::QUEEN;
            else if (buttonY >= 260 && buttonY < 310) selectedType = PieceType::ROOK;
            else if (buttonY >= 320 && buttonY < 370) selectedType = PieceType::BISHOP;
            else if (buttonY >= 380 && buttonY < 430) selectedType = PieceType::KNIGHT;
            else return; // Click outside buttons
            
            // Promote the pawn
            board->promotePawn(promotionSquare, selectedType);
            std::cout << "Pawn promoted!\n";
            
            waitingForPromotion = false;
            promotionSquare = Position<int>(-1, -1);
            return;
        }
        return; // Ignore board clicks during promotion
    }
    
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
            Piece* selectedPiece = board->getPieceAt(selectedSquare);
            Piece* capturedPiece = board->getPieceAt(clickedSquare);
            bool isCapture = capturedPiece != nullptr && capturedPiece->getColor() != board->getCurrentTurn();
            
            // Detect castling move
            bool isCastling = false;
            if (selectedPiece && selectedPiece->getType() == PieceType::KING) {
                int colDiff = std::abs(clickedSquare.getCol() - selectedSquare.getCol());
                if (colDiff == 2) {
                    isCastling = true;
                }
            }

            if (board->movePiece(selectedSquare, clickedSquare)) {
                if (isCastling) {
                    std::cout << "Castling performed! ";
                    if (clickedSquare.getCol() > selectedSquare.getCol()) {
                        std::cout << "Kingside (O-O)\n";
                    } else {
                        std::cout << "Queenside (O-O-O)\n";
                    }
                } else {
                    std::cout << "Move made: (" << selectedSquare.getRow() 
                             << "," << selectedSquare.getCol() << ") -> ("
                             << clickedSquare.getRow() << "," << clickedSquare.getCol() << ")\n";
                }
                
                // Add to history
                history->addMove(Move(selectedSquare, clickedSquare, isCapture, isCastling));
                
                // Check for pawn promotion
                Piece* movedPiece = board->getPieceAt(clickedSquare);
                if (movedPiece && movedPiece->getType() == PieceType::PAWN) {
                    int endRank = (movedPiece->getColor() == PieceColor::WHITE) ? 0 : 7;
                    if (clickedSquare.getRow() == endRank) {
                        // Show promotion UI
                        waitingForPromotion = true;
                        promotionSquare = clickedSquare;
                        promotionColor = movedPiece->getColor();
                        std::cout << "Choose promotion piece!\n";
                        isPieceSelected = false;
                        currentValidMoves.clear();
                        return; // Don't check for checkmate yet
                    }
                }
                
                // After movePiece(), the turn has already switched!
                // So currentTurn is now the OPPONENT who just received the turn.
                PieceColor opponentColor = board->getCurrentTurn();
                
                // Check for checkmate  
                if (board->isPlayerInCheckmate(opponentColor)) {
                    gameOver = true;
                    std::string winner = (opponentColor == PieceColor::WHITE ? blackPlayerName : whitePlayerName);
                    gameResult = winner + " wins by checkmate!";
                    std::cout << "\n*** CHECKMATE! " << winner << " wins! ***\n\n";
                    saveMatchResult("Checkmate");
                }
                // Check for stalemate
                else if (board->isPlayerInStalemate(opponentColor)) {
                    gameOver = true;
                    gameResult = "Game drawn by stalemate";
                    std::cout << "\n*** STALEMATE! Game is a draw! ***\n\n";
                    saveMatchResult("Stalemate");
                }
                // Check for draw by insufficient material
                else if (board->hasInsufficientMaterial()) {
                    gameOver = true;
                    gameResult = "Game drawn by insufficient material";
                    std::cout << "\n*** DRAW! Insufficient material to checkmate. ***\n\n";
                    saveMatchResult("Draw - Insufficient Material");
                }
                // Check for draw by 50-move rule
                else if (board->isFiftyMoveRule()) {
                    gameOver = true;
                    gameResult = "Game drawn by fifty-move rule";
                    std::cout << "\n*** DRAW! 50 moves without capture or pawn move. ***\n\n";
                    saveMatchResult("Draw - Fifty Move Rule");
                }
                // Check for draw by threefold repetition
                else if (board->isThreefoldRepetition()) {
                    gameOver = true;
                    gameResult = "Game drawn by threefold repetition";
                    std::cout << "\n*** DRAW! Same position repeated three times. ***\n\n";
                    saveMatchResult("Draw - Threefold Repetition");
                }
                // Check for check
                else if (board->isPlayerInCheck(opponentColor)) {
                    isCheck = true;
                    std::string playerName = (opponentColor == PieceColor::WHITE ? whitePlayerName : blackPlayerName);
                    std::cout << playerName << " is in CHECK!\n";
                }
                else {
                    isCheck = false;
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

void Game::render() {
    window.clear(sf::Color(49, 46, 43));  // Chess.com dark background
    
    // Draw board and pieces with move hints
    board->draw(window, uiFont, currentValidMoves);

    // Draw visual check warning - RED BORDER around king
    if (isCheck && !gameOver) {
        Position<int> kingPos = board->findKing(board->getCurrentTurn());
        if (kingPos.isValid()) {
            // Draw thick red border around king square
            sf::RectangleShape checkWarning(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
            checkWarning.setPosition(sf::Vector2f(
                static_cast<float>(BORDER_SIZE + kingPos.getCol() * SQUARE_SIZE),
                static_cast<float>(BORDER_SIZE + kingPos.getRow() * SQUARE_SIZE)
            ));
            checkWarning.setFillColor(sf::Color::Transparent);
            checkWarning.setOutlineColor(sf::Color::Red);
            checkWarning.setOutlineThickness(6.0f); // Thick red border
            window.draw(checkWarning);
        }
    }

    // Highlight selected square with Chess.com style (green highlight)
    if (isPieceSelected) {
        sf::RectangleShape highlight(sf::Vector2f(static_cast<float>(SQUARE_SIZE), static_cast<float>(SQUARE_SIZE)));
        highlight.setPosition(sf::Vector2f(static_cast<float>(BORDER_SIZE + selectedSquare.getCol() * SQUARE_SIZE), 
                                          static_cast<float>(BORDER_SIZE + selectedSquare.getRow() * SQUARE_SIZE)));
        // Chess.com style: Yellow-green highlight
        highlight.setFillColor(sf::Color(186, 202, 68, 150));  // Semi-transparent green
        highlight.setOutlineColor(sf::Color(186, 202, 68));
        highlight.setOutlineThickness(3);
        window.draw(highlight);
    }
    
    // Draw UI (player info and timer) on the right side
    if (uiFontLoaded) {
        float uiX = BOARD_SIZE * SQUARE_SIZE + 2 * BORDER_SIZE + 10;
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
        separator.setPosition(sf::Vector2f(uiX, (BOARD_SIZE * SQUARE_SIZE + 2 * BORDER_SIZE) / 2 - 20));
        separator.setFillColor(sf::Color(80, 80, 80));
        window.draw(separator);
        
        // White player info (bottom)
        float whiteY = BOARD_SIZE * SQUARE_SIZE + 2 * BORDER_SIZE - 100;
        
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
            resultText.setPosition(sf::Vector2f(uiX, (BOARD_SIZE * SQUARE_SIZE + 2 * BORDER_SIZE) / 2 + 20));
            window.draw(resultText);
            
            // Winner message
            sf::Text winnerText(uiFont);
            winnerText.setString(gameResult);
            winnerText.setCharacterSize(14);
            winnerText.setFillColor(sf::Color::Yellow);
            winnerText.setPosition(sf::Vector2f(uiX, (BOARD_SIZE * SQUARE_SIZE + 2 * BORDER_SIZE) / 2 + 55));
            window.draw(winnerText);
        }
        // Check warning text
        else if (isCheck && !gameOver) {
            sf::Text checkText(uiFont);
            checkText.setString("CHECK!");
            checkText.setCharacterSize(28);
            checkText.setFillColor(sf::Color::Red);
            checkText.setStyle(sf::Text::Bold);
            checkText.setPosition(sf::Vector2f(uiX, (BOARD_SIZE * SQUARE_SIZE + 2 * BORDER_SIZE) / 2 - 20));
            window.draw(checkText);
        }
        
        // Pawn promotion UI
        if (waitingForPromotion) {
            // Semi-transparent overlay
            sf::RectangleShape overlay(sf::Vector2f(200, BOARD_SIZE * SQUARE_SIZE + 2 * BORDER_SIZE));
            overlay.setPosition(sf::Vector2f(BOARD_SIZE * SQUARE_SIZE + 2 * BORDER_SIZE, 0));
            overlay.setFillColor(sf::Color(0, 0, 0, 180));
            window.draw(overlay);
            
            // Title
            sf::Text titleText(uiFont);
            titleText.setString("Promote Pawn");
            titleText.setCharacterSize(20);
            titleText.setFillColor(sf::Color::White);
            titleText.setStyle(sf::Text::Bold);
            titleText.setPosition(sf::Vector2f(uiX, 150));
            window.draw(titleText);
            
            // Promotion buttons
            std::vector<std::pair<std::string, int>> buttons = {
                {"Queen", 200},
                {"Rook", 260},
                {"Bishop", 320},
                {"Knight", 380}
            };
            
            for (const auto& btn : buttons) {
                // Button background
                sf::RectangleShape button(sf::Vector2f(180.f, 45.f));
                button.setPosition(sf::Vector2f(static_cast<float>(uiX), static_cast<float>(btn.second)));
                button.setFillColor(sf::Color(70, 130, 180));
                button.setOutlineColor(sf::Color::White);
                button.setOutlineThickness(2.f);
                window.draw(button);
                
                // Button text
                sf::Text buttonText(uiFont);
                buttonText.setString(btn.first);
                buttonText.setCharacterSize(18);
                buttonText.setFillColor(sf::Color::White);
                buttonText.setStyle(sf::Text::Bold);
                buttonText.setPosition(sf::Vector2f(static_cast<float>(uiX + 50), static_cast<float>(btn.second + 12)));
                window.draw(buttonText);
            }
        }
    }

    window.display();
}

// Public methods
Game::Game() : 
    board(std::make_unique<Board>()),
    history(std::make_unique<GameHistory>()),
    selectedSquare(-1, -1),
    isPieceSelected(false),
    whiteTimeRemaining(600),  // 10 minutes = 600 seconds
    blackTimeRemaining(600),
    timerEnabled(true),
    gameOver(false),
    isCheck(false),
    waitingForPromotion(false),
    promotionSquare(-1, -1),
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
    window.create(sf::VideoMode({static_cast<unsigned int>(LOGICAL_WIDTH), static_cast<unsigned int>(LOGICAL_HEIGHT)}), 
                 "Chess Game", 
                 sf::Style::Default);
    window.setFramerateLimit(60);
    
    // Initialize view
    gameView.setSize(sf::Vector2f(LOGICAL_WIDTH, LOGICAL_HEIGHT));
    gameView.setCenter(sf::Vector2f(LOGICAL_WIDTH / 2.0f, LOGICAL_HEIGHT / 2.0f));
    window.setView(gameView);
    
    // Record game start time
    gameStartTime = std::time(nullptr);
    
    // Start the clocks
    gameClock.restart();
    turnClock.restart();
}

Game::~Game() {
    try {
        history->saveToFile();
        std::cout << "Game history saved!\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

void Game::run() {
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
