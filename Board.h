#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "Piece.h"
#include "Pieces.h"
#include "Position.h"
#include "Move.h"
#include "Enums.h"

const int BOARD_SIZE = 8;
const int SQUARE_SIZE = 80;
const int BORDER_SIZE = 40; // Space for coordinates

class Board {
private:
    Piece* board[BOARD_SIZE][BOARD_SIZE];
    std::vector<std::unique_ptr<Piece>> pieces;
    PieceColor currentTurn;
    std::map<std::string, sf::Texture> textures;
    sf::Texture woodTexture;
    bool texturesLoaded;
    bool woodTextureLoaded;
    
    // En passant tracking
    Position<int> lastMoveFrom;
    Position<int> lastMoveTo;
    Piece* lastMovedPiece;
    
    // Draw condition tracking
    int halfmoveClock;  // For 50-move rule
    std::map<std::string, int> positionHistory;  // For threefold repetition

    std::string getPieceKey(PieceType type, PieceColor color) const {
        std::string colorSuffix = (color == PieceColor::WHITE) ? "W" : "B";
        std::string typeStr;
        
        switch (type) {
            case PieceType::KING: typeStr = "King"; break;
            case PieceType::QUEEN: typeStr = "Queen"; break;
            case PieceType::ROOK: typeStr = "Rook"; break;
            case PieceType::BISHOP: typeStr = "Bishop"; break;
            case PieceType::KNIGHT: typeStr = "Knight"; break;
            case PieceType::PAWN: typeStr = "Pawn"; break;
            default: typeStr = "unknown"; break;
        }
        
        return typeStr + colorSuffix;
    }

    bool loadTextures() {
        std::string assetPath = "assets/";
        
        std::vector<std::string> pieceFiles = {
            "KingW.png", "QueenW.png", "RookW.png",
            "BishopW.png", "KnightW.png", "PawnW.png",
            "KingB.png", "QueenB.png", "RookB.png",
            "BishopB.png", "KnightB.png", "PawnB.png"
        };

        bool allLoaded = true;
        std::cout << "\n=== Loading Chess Piece Textures ===\n";
        
        for (const auto& filename : pieceFiles) {
            std::string fullPath = assetPath + filename;
            
            // SFML 3.0: Create texture and load from file
            sf::Texture texture;
            if (texture.loadFromFile(fullPath)) {
                // Extract key from filename (remove .png)
                std::string key = filename.substr(0, filename.length() - 4);
                textures[key] = std::move(texture);
                
                // Make texture smooth for better scaling
                textures[key].setSmooth(true);
                
                std::cout << "? Loaded: " << filename 
                          << " (Size: " << textures[key].getSize().x 
                          << "x" << textures[key].getSize().y << ")" << std::endl;
            } else {
                allLoaded = false;
                std::cerr << "? Failed to load: " << fullPath << std::endl;
            }
        }
        
        std::cout << "=================================\n\n";
        std::cout << "=================================\n\n";
        return allLoaded;
    }

    void loadWoodTexture() {
        if (woodTexture.loadFromFile("assets/WoodTexture.png")) {
            woodTexture.setSmooth(true);
            woodTexture.setRepeated(true);
            woodTextureLoaded = true;
            std::cout << "Loaded WoodTexture.png\n";
        } else {
            woodTextureLoaded = false;
            std::cout << "Failed to load WoodTexture.png, using fallback color.\n";
        }
    }

public:
    Board() : currentTurn(PieceColor::WHITE), texturesLoaded(false), 
              lastMoveFrom(-1, -1), lastMoveTo(-1, -1), lastMovedPiece(nullptr),
              halfmoveClock(0) {
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                board[i][j] = nullptr;
            }
        }
        initializeBoard();
        texturesLoaded = loadTextures();
        loadWoodTexture();
        assignTextures(); // Assign textures after loading
        
        if (texturesLoaded) {
            std::cout << "Successfully loaded all chess piece images!" << std::endl;
        } else {
            std::cout << "Warning: Some piece images failed to load. Using fallback graphics." << std::endl;
        }
    }

    ~Board() {
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                board[i][j] = nullptr;
            }
        }
    }

    void assignTextures() {
        if (!texturesLoaded) return;
        
        for (auto& piece : pieces) {
            std::string key = getPieceKey(piece->getType(), piece->getColor());
            auto it = textures.find(key);
            if (it != textures.end()) {
                piece->setTexture(&it->second);
            }
        }
    }

    void initializeBoard() {
        pieces.clear(); // Clear existing pieces first
        
        // Black pieces
        pieces.push_back(std::make_unique<Rook>(PieceColor::BLACK, Position<int>(0, 0)));
        pieces.push_back(std::make_unique<Knight>(PieceColor::BLACK, Position<int>(0, 1)));
        pieces.push_back(std::make_unique<Bishop>(PieceColor::BLACK, Position<int>(0, 2)));
        pieces.push_back(std::make_unique<Queen>(PieceColor::BLACK, Position<int>(0, 3)));
        pieces.push_back(std::make_unique<King>(PieceColor::BLACK, Position<int>(0, 4)));
        pieces.push_back(std::make_unique<Bishop>(PieceColor::BLACK, Position<int>(0, 5)));
        pieces.push_back(std::make_unique<Knight>(PieceColor::BLACK, Position<int>(0, 6)));
        pieces.push_back(std::make_unique<Rook>(PieceColor::BLACK, Position<int>(0, 7)));

        for (int i = 0; i < BOARD_SIZE; i++) {
            pieces.push_back(std::make_unique<Pawn>(PieceColor::BLACK, Position<int>(1, i)));
        }

        // White pieces
        for (int i = 0; i < BOARD_SIZE; i++) {
            pieces.push_back(std::make_unique<Pawn>(PieceColor::WHITE, Position<int>(6, i)));
        }

        pieces.push_back(std::make_unique<Rook>(PieceColor::WHITE, Position<int>(7, 0)));
        pieces.push_back(std::make_unique<Knight>(PieceColor::WHITE, Position<int>(7, 1)));
        pieces.push_back(std::make_unique<Bishop>(PieceColor::WHITE, Position<int>(7, 2)));
        pieces.push_back(std::make_unique<Queen>(PieceColor::WHITE, Position<int>(7, 3)));
        pieces.push_back(std::make_unique<King>(PieceColor::WHITE, Position<int>(7, 4)));
        pieces.push_back(std::make_unique<Bishop>(PieceColor::WHITE, Position<int>(7, 5)));
        pieces.push_back(std::make_unique<Knight>(PieceColor::WHITE, Position<int>(7, 6)));
        pieces.push_back(std::make_unique<Rook>(PieceColor::WHITE, Position<int>(7, 7)));

        assignTextures(); // Cache texture pointers
        updateBoardArray();
    }

    void updateBoardArray() {
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                board[i][j] = nullptr;
            }
        }

        for (const auto& piece : pieces) {
            Position<int> pos = piece->getPosition();
            board[pos.getRow()][pos.getCol()] = piece.get();
        }
    }

    // Helper to simulate a move without actually making it
    // Helper to simulate a move without actually making it
    bool simulateMove(Position<int> from, Position<int> to, PieceColor& opponentColor) {
        Piece* movingPiece = board[from.getRow()][from.getCol()];
        Piece* capturedPiece = board[to.getRow()][to.getCol()];
        
        if (movingPiece == nullptr) return true; // Invalid
        
        // Save original positions
        Position<int> originalMovingPos = movingPiece->getPosition();
        Position<int> originalCapturedPos(-1, -1);
        
        if (capturedPiece != nullptr) {
            originalCapturedPos = capturedPiece->getPosition();
        }
        
        // Make temporary move
        board[to.getRow()][to.getCol()] = movingPiece;
        board[from.getRow()][from.getCol()] = nullptr;
        movingPiece->setPosition(to);
        
        // CRITICAL FIX: Move captured piece to invalid position so it doesn't attack during check detection
        if (capturedPiece != nullptr) {
            capturedPiece->setPosition(Position<int>(-1, -1));
        }
        
        // Check if this leaves our king in check
        bool leavesKingInCheck = isPlayerInCheck(movingPiece->getColor());
        
        // CRITICAL: Restore everything back
        movingPiece->setPosition(originalMovingPos);
        board[from.getRow()][from.getCol()] = movingPiece;
        board[to.getRow()][to.getCol()] = capturedPiece;
        
        if (capturedPiece != nullptr) {
            capturedPiece->setPosition(originalCapturedPos);
        }
        
        return leavesKingInCheck;
    }
    
    bool wouldLeaveKingInCheck(Position<int> from, Position<int> to) {
        if (!from.isValid() || !to.isValid()) return true;
        
        Piece* piece = board[from.getRow()][from.getCol()];
        if (piece == nullptr) return true;
        
        PieceColor opponentColor = (piece->getColor() == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
        return simulateMove(from, to, opponentColor);
    }

    bool movePiece(Position<int> from, Position<int> to) {
        if (!from.isValid() || !to.isValid()) return false;

        Piece* piece = board[from.getRow()][from.getCol()];
        if (piece == nullptr || piece->getColor() != currentTurn) return false;

        if (!piece->isValidMove(to, board)) return false;
        
        // Check if this is a castling move
        bool isCastling = false;
        Position<int> rookFrom(-1, -1);
        Position<int> rookTo(-1, -1);
        
        if (piece->getType() == PieceType::KING) {
            int colDiff = to.getCol() - from.getCol();
            if (std::abs(colDiff) == 2) {
                isCastling = true;
                int row = from.getRow();
                
                // Determine rook positions
                if (colDiff > 0) {
                    // Kingside castling
                    rookFrom = Position<int>(row, 7);
                    rookTo = Position<int>(row, 5);
                } else {
                    // Queenside castling
                    rookFrom = Position<int>(row, 0);
                    rookTo = Position<int>(row, 3);
                }
                
                // For castling, we need to check that the king doesn't pass through check
                // Check the intermediate square
                int intermediateCol = from.getCol() + (colDiff > 0 ? 1 : -1);
                Position<int> intermediatePos(row, intermediateCol);
                
                if (wouldLeaveKingInCheck(from, intermediatePos)) {
                    return false; // King would pass through check
                }
            }
        }
        
        // CRITICAL: Check if this move would leave the king in check
        if (wouldLeaveKingInCheck(from, to)) {
            return false; // Illegal move!
        }

        // Check for en passant capture
        bool isEnPassant = false;
        Position<int> enPassantCapturePos(-1, -1);
        
        if (piece->getType() == PieceType::PAWN) {
            int colDiff = std::abs(to.getCol() - from.getCol());
            // Diagonal move to empty square = en passant
            if (colDiff == 1 && board[to.getRow()][to.getCol()] == nullptr) {
                isEnPassant = true;
                // The captured pawn is on the same row as the moving pawn, but in the target column
                enPassantCapturePos = Position<int>(from.getRow(), to.getCol());
            }
        }

        // Handle regular capture
        Piece* targetPiece = board[to.getRow()][to.getCol()];
        if (targetPiece != nullptr) {
            pieces.erase(
                std::remove_if(pieces.begin(), pieces.end(),
                    [targetPiece](const std::unique_ptr<Piece>& p) {
                        return p.get() == targetPiece;
                    }),
                pieces.end()
            );
        }
        
        // Handle en passant capture
        if (isEnPassant && enPassantCapturePos.isValid()) {
            Piece* capturedPawn = board[enPassantCapturePos.getRow()][enPassantCapturePos.getCol()];
            if (capturedPawn != nullptr) {
                pieces.erase(
                    std::remove_if(pieces.begin(), pieces.end(),
                        [capturedPawn](const std::unique_ptr<Piece>& p) {
                            return p.get() == capturedPawn;
                        }),
                    pieces.end()
                );
            }
        }

        piece->setPosition(to);
        piece->setHasMoved(true);
        
        // Handle castling: move the rook
        if (isCastling && rookFrom.isValid() && rookTo.isValid()) {
            Piece* rook = board[rookFrom.getRow()][rookFrom.getCol()];
            if (rook != nullptr) {
                rook->setPosition(rookTo);
                rook->setHasMoved(true);
            }
        }
        
        updateBoardArray();
        
        // Update halfmove clock for 50-move rule
        if (piece->getType() == PieceType::PAWN || targetPiece != nullptr || isEnPassant) {
            halfmoveClock = 0;  // Reset on pawn move or capture
            positionHistory.clear();  // Also reset position history
        } else {
            halfmoveClock++;
        }
        
        // Track last move for en passant validation
        lastMoveFrom = from;
        lastMoveTo = to;
        lastMovedPiece = piece;

        currentTurn = (currentTurn == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
        
        // Track position for threefold repetition (after turn switch)
        std::string posHash = getPositionHash();
        positionHistory[posHash]++;

        return true;
    }
    
    // Promote pawn to specified piece type
    bool promotePawn(Position<int> position, PieceType newType) {
        Piece* piece = getPieceAt(position);
        if (piece == nullptr || piece->getType() != PieceType::PAWN) {
            return false;
        }
        
        PieceColor color = piece->getColor();
        
        // Remove the pawn
        pieces.erase(
            std::remove_if(pieces.begin(), pieces.end(),
                [piece](const std::unique_ptr<Piece>& p) {
                    return p.get() == piece;
                }),
            pieces.end()
        );
        
        // Add the new piece
        std::unique_ptr<Piece> newPiece;
        switch (newType) {
            case PieceType::QUEEN:
                newPiece = std::make_unique<Queen>(color, position);
                break;
            case PieceType::ROOK:
                newPiece = std::make_unique<Rook>(color, position);
                break;
            case PieceType::BISHOP:
                newPiece = std::make_unique<Bishop>(color, position);
                break;
            case PieceType::KNIGHT:
                newPiece = std::make_unique<Knight>(color, position);
                break;
            default:
                return false;
        }
        
        // Assign texture to new piece
        if (texturesLoaded) {
            std::string key = getPieceKey(newType, color);
            auto it = textures.find(key);
            if (it != textures.end()) {
                newPiece->setTexture(&it->second);
            }
        }
        
        pieces.push_back(std::move(newPiece));
        updateBoardArray();
        
        return true;
    }

    void draw(sf::RenderWindow& window, const sf::Font& font, const std::vector<Position<int>>& validMoves = std::vector<Position<int>>()) const {
        // Chess.com style colors
        sf::Color lightSquare(240, 217, 181);
        sf::Color darkSquare(181, 136, 99);
        sf::Color borderColor(101, 67, 33); // Dark brown fallback
        
        // Draw Border
        sf::RectangleShape borderRect(sf::Vector2f(
            static_cast<float>(BOARD_SIZE * SQUARE_SIZE + 2 * BORDER_SIZE), 
            static_cast<float>(BOARD_SIZE * SQUARE_SIZE + 2 * BORDER_SIZE)
        ));
        
        if (woodTextureLoaded) {
            borderRect.setTexture(&woodTexture);
            // Calculate texture rect to tile it if needed, or just stretch
            // For a simple border, stretching might be okay, or using a large texture
        } else {
            borderRect.setFillColor(borderColor);
        }
        window.draw(borderRect);

        // Draw Coordinates
        sf::Text coordText(font);
        coordText.setCharacterSize(20);
        coordText.setFillColor(sf::Color(240, 217, 181)); // Light color for text
        coordText.setStyle(sf::Text::Bold);

        // Letters (a-h) - Bottom and Top
        for (int col = 0; col < BOARD_SIZE; col++) {
            char letter = 'a' + col;
            coordText.setString(std::string(1, letter));
            
            // Center text in the square width
            sf::FloatRect bounds = coordText.getLocalBounds();
            float x = BORDER_SIZE + col * SQUARE_SIZE + (SQUARE_SIZE - bounds.size.x) / 2.0f;
            
            // Top
            coordText.setPosition(sf::Vector2f(x, (BORDER_SIZE - bounds.size.y) / 2.0f - 5));
            window.draw(coordText);
            
            // Bottom
            coordText.setPosition(sf::Vector2f(x, BORDER_SIZE + BOARD_SIZE * SQUARE_SIZE + (BORDER_SIZE - bounds.size.y) / 2.0f - 5));
            window.draw(coordText);
        }

        // Numbers (1-8) - Left and Right
        for (int row = 0; row < BOARD_SIZE; row++) {
            std::string num = std::to_string(8 - row);
            coordText.setString(num);
            
            sf::FloatRect bounds = coordText.getLocalBounds();
            float y = BORDER_SIZE + row * SQUARE_SIZE + (SQUARE_SIZE - bounds.size.y) / 2.0f;
            
            // Left
            coordText.setPosition(sf::Vector2f((BORDER_SIZE - bounds.size.x) / 2.0f, y - 5));
            window.draw(coordText);
            
            // Right
            coordText.setPosition(sf::Vector2f(BORDER_SIZE + BOARD_SIZE * SQUARE_SIZE + (BORDER_SIZE - bounds.size.x) / 2.0f, y - 5));
            window.draw(coordText);
        }

        // Reusable shapes to avoid per-frame allocation
        static sf::RectangleShape square(sf::Vector2f(static_cast<float>(SQUARE_SIZE), static_cast<float>(SQUARE_SIZE)));
        static sf::CircleShape fallback(SQUARE_SIZE * 0.3f);
        
        // Draw chessboard
        for (int row = 0; row < BOARD_SIZE; row++) {
            for (int col = 0; col < BOARD_SIZE; col++) {
                // Offset by BORDER_SIZE
                float xPos = static_cast<float>(BORDER_SIZE + col * SQUARE_SIZE);
                float yPos = static_cast<float>(BORDER_SIZE + row * SQUARE_SIZE);
                
                square.setPosition(sf::Vector2f(xPos, yPos));

                if ((row + col) % 2 == 0) {
                    square.setFillColor(lightSquare);
                } else {
                    square.setFillColor(darkSquare);
                }

                window.draw(square);

                // Draw piece if there is one
                if (board[row][col] != nullptr) {
                    Piece* piece = board[row][col];
                    sf::Texture* texture = piece->getTexture();
                    
                    if (texture != nullptr) {
                        sf::Sprite sprite(*texture);
                        
                        // Scale the sprite to fit the square
                        sf::Vector2u textureSize = texture->getSize();
                        float scaleX = static_cast<float>(SQUARE_SIZE) / textureSize.x;
                        float scaleY = static_cast<float>(SQUARE_SIZE) / textureSize.y;
                        sprite.setScale(sf::Vector2f(scaleX, scaleY));
                        
                        sprite.setPosition(sf::Vector2f(xPos, yPos));
                        window.draw(sprite);
                    } else if (!texturesLoaded) {
                        // Fallback: draw a simple colored circle if texture not loaded
                        fallback.setPosition(sf::Vector2f(
                            xPos + SQUARE_SIZE * 0.2f,
                            yPos + SQUARE_SIZE * 0.2f
                        ));
                        fallback.setFillColor(
                            piece->getColor() == PieceColor::WHITE ? 
                            sf::Color(255, 255, 255, 200) : 
                            sf::Color(50, 50, 50, 200)
                        );
                        window.draw(fallback);
                    }
                }
            }
        }
        
        // Draw move hints (Chess.com style)
        for (const auto& move : validMoves) {
            if (!move.isValid()) continue;
            
            float centerX = BORDER_SIZE + move.getCol() * SQUARE_SIZE + SQUARE_SIZE / 2.0f;
            float centerY = BORDER_SIZE + move.getRow() * SQUARE_SIZE + SQUARE_SIZE / 2.0f;
            
            Piece* targetPiece = board[move.getRow()][move.getCol()];
            
            if (targetPiece == nullptr) {
                // Empty square - draw small dot
                sf::CircleShape dot(SQUARE_SIZE * 0.15f);
                dot.setOrigin(sf::Vector2f(SQUARE_SIZE * 0.15f, SQUARE_SIZE * 0.15f));
                dot.setPosition(sf::Vector2f(centerX, centerY));
                dot.setFillColor(sf::Color(0, 0, 0, 100));
                window.draw(dot);
            } else {
                // Capture - draw ring
                sf::CircleShape ring(SQUARE_SIZE * 0.45f);
                ring.setOrigin(sf::Vector2f(SQUARE_SIZE * 0.45f, SQUARE_SIZE * 0.45f));
                ring.setPosition(sf::Vector2f(centerX, centerY));
                ring.setFillColor(sf::Color::Transparent);
                ring.setOutlineColor(sf::Color(0, 0, 0, 120));
                ring.setOutlineThickness(4.0f);
                window.draw(ring);
            }
        }
    }
    
    std::vector<Position<int>> getValidMoves(Position<int> from) const {
        std::vector<Position<int>> moves;
        Piece* piece = getPieceAt(from);
        if (piece == nullptr) return moves;
        
        // Get all pseudo-legal moves
        for (int row = 0; row < BOARD_SIZE; row++) {
            for (int col = 0; col < BOARD_SIZE; col++) {
                Position<int> to(row, col);
                // C-style cast to non-const board for isValidMove
                if (piece->isValidMove(to, (Piece*(*)[8])board)) {
                    // Only add if it doesn't leave king in check
                    // NOTE: wouldLeaveKingInCheck does its own simulation, 
                    // so we don't need to update the board array here
                    if (!const_cast<Board*>(this)->wouldLeaveKingInCheck(from, to)) {
                        moves.push_back(to);
                    }
                }
            }
        }
        return moves;
    }
    
    Position<int> findKing(PieceColor color) const {
        for (const auto& piece : pieces) {
            if (piece->getType() == PieceType::KING && piece->getColor() == color) {
                return piece->getPosition();
            }
        }
        return Position<int>(-1, -1);
    }
    
    bool isSquareUnderAttack(Position<int> square, PieceColor attackingColor) const {
        for (const auto& piece : pieces) {
            if (piece->getColor() == attackingColor) {
                if (piece->isValidMove(square, (Piece*(*)[8])board)) {
                    return true;
                }
            }
        }
        return false;
    }

    bool isPlayerInCheck(PieceColor color) const {
        Position<int> kingPos = findKing(color);
        if (!kingPos.isValid()) return false;
        
        PieceColor opponentColor = (color == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
        return isSquareUnderAttack(kingPos, opponentColor);
    }
    
    
    bool hasAnyValidMoves(PieceColor color) const {
        for (const auto& piece : pieces) {
            if (piece->getColor() == color) {
                std::vector<Position<int>> moves = getValidMoves(piece->getPosition());
                if (!moves.empty()) {
                    return true;
                }
            }
        }
        return false;
    }
    
    bool isPlayerInCheckmate(PieceColor color) const {
        if (!isPlayerInCheck(color)) return false;
        return !hasAnyValidMoves(color);
    }
    
    bool isPlayerInStalemate(PieceColor color) const {
        if (isPlayerInCheck(color)) return false;
        return !hasAnyValidMoves(color);
    }
    
    // Draw by insufficient material
    bool hasInsufficientMaterial() const {
        std::vector<PieceType> whitePieces, blackPieces;
        
        for (const auto& piece : pieces) {
            if (piece->getType() != PieceType::KING) {
                if (piece->getColor() == PieceColor::WHITE) {
                    whitePieces.push_back(piece->getType());
                } else {
                    blackPieces.push_back(piece->getType());
                }
            }
        }
        
        // King vs King
        if (whitePieces.empty() && blackPieces.empty()) return true;
        
        // King + Bishop vs King or King vs King + Bishop
        if ((whitePieces.size() == 1 && whitePieces[0] == PieceType::BISHOP && blackPieces.empty()) ||
            (blackPieces.size() == 1 && blackPieces[0] == PieceType::BISHOP && whitePieces.empty())) {
            return true;
        }
        
        // King + Knight vs King or King vs King + Knight
        if ((whitePieces.size() == 1 && whitePieces[0] == PieceType::KNIGHT && blackPieces.empty()) ||
            (blackPieces.size() == 1 && blackPieces[0] == PieceType::KNIGHT && whitePieces.empty())) {
            return true;
        }
        
        return false;
    }
    
    // 50-move rule (100 halfmoves)
    bool isFiftyMoveRule() const {
        return halfmoveClock >= 100;
    }
    
    // Threefold repetition
    bool isThreefoldRepetition() const {
        for (const auto& entry : positionHistory) {
            if (entry.second >= 3) {
                return true;
            }
        }
        return false;
    }
    
    // Generate position hash for repetition tracking
    std::string getPositionHash() const {
        std::string hash;
        for (int row = 0; row < BOARD_SIZE; row++) {
            for (int col = 0; col < BOARD_SIZE; col++) {
                Piece* piece = board[row][col];
                if (piece == nullptr) {
                    hash += ".";
                } else {
                    hash += piece->getSymbol();
                }
            }
        }
        hash += (currentTurn == PieceColor::WHITE) ? "W" : "B";
        return hash;
    }
    
    int getMaterialScore(PieceColor color) const {
        int score = 0;
        for (const auto& piece : pieces) {
            if (piece->getColor() == color) {
                switch (piece->getType()) {
                    case PieceType::PAWN: score += 1; break;
                    case PieceType::KNIGHT: score += 3; break;
                    case PieceType::BISHOP: score += 3; break;
                    case PieceType::ROOK: score += 5; break;
                    case PieceType::QUEEN: score += 9; break;
                    case PieceType::KING: score += 0; break;
                }
            }
        }
        return score;
    }

    Piece* getPieceAt(Position<int> pos) const {
        if (!pos.isValid()) return nullptr;
        return board[pos.getRow()][pos.getCol()];
    }

    PieceColor getCurrentTurn() const {
        return currentTurn;
    }

    int countPieces(PieceColor color) const {
        return static_cast<int>(std::count_if(pieces.begin(), pieces.end(),
            [color](const std::unique_ptr<Piece>& piece) {
                return piece->getColor() == color;
            }));
    }

    typedef bool (*MoveValidator)(const Piece*, Position<int>, Piece* board[8][8]);

    bool validateMoveWithFunction(Position<int> from, Position<int> to, MoveValidator validator) {
        Piece* piece = getPieceAt(from);
        if (piece == nullptr) return false;
        return validator(piece, to, board);
    }

    const std::vector<std::unique_ptr<Piece>>& getPieces() const {
        return pieces;
    }
};
