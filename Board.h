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

class Board {
private:
    Piece* board[BOARD_SIZE][BOARD_SIZE];
    std::vector<std::unique_ptr<Piece>> pieces;
    PieceColor currentTurn;
    std::map<std::string, sf::Texture> textures;
    bool texturesLoaded;

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
        return allLoaded;
    }

public:
    Board() : currentTurn(PieceColor::WHITE), texturesLoaded(false) {
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                board[i][j] = nullptr;
            }
        }
        initializeBoard();
        texturesLoaded = loadTextures();
        
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

    void initializeBoard() {
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

    bool movePiece(Position<int> from, Position<int> to) {
        if (!from.isValid() || !to.isValid()) return false;

        Piece* piece = board[from.getRow()][from.getCol()];
        if (piece == nullptr || piece->getColor() != currentTurn) return false;

        if (!piece->isValidMove(to, board)) return false;

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

        piece->setPosition(to);
        piece->setHasMoved(true);
        updateBoardArray();

        currentTurn = (currentTurn == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;

        return true;
    }

    void draw(sf::RenderWindow& window) const {
        // Chess.com style colors
        sf::Color lightSquare(240, 217, 181);
        sf::Color darkSquare(181, 136, 99);
        
        // Draw chessboard
        for (int row = 0; row < BOARD_SIZE; row++) {
            for (int col = 0; col < BOARD_SIZE; col++) {
                sf::RectangleShape square(sf::Vector2f(static_cast<float>(SQUARE_SIZE), static_cast<float>(SQUARE_SIZE)));
                square.setPosition(sf::Vector2f(static_cast<float>(col * SQUARE_SIZE), static_cast<float>(row * SQUARE_SIZE)));

                if ((row + col) % 2 == 0) {
                    square.setFillColor(lightSquare);
                } else {
                    square.setFillColor(darkSquare);
                }

                window.draw(square);

                // Draw piece if there is one
                if (board[row][col] != nullptr) {
                    Piece* piece = board[row][col];
                    std::string key = getPieceKey(piece->getType(), piece->getColor());
                    
                    auto it = textures.find(key);
                    if (it != textures.end()) {
                        sf::Sprite sprite(it->second);
                        
                        // Scale the sprite to fit the square
                        sf::Vector2u textureSize = it->second.getSize();
                        float scaleX = static_cast<float>(SQUARE_SIZE) / textureSize.x;
                        float scaleY = static_cast<float>(SQUARE_SIZE) / textureSize.y;
                        sprite.setScale(sf::Vector2f(scaleX, scaleY));
                        
                        sprite.setPosition(sf::Vector2f(
                            static_cast<float>(col * SQUARE_SIZE),
                            static_cast<float>(row * SQUARE_SIZE)
                        ));
                        window.draw(sprite);
                    } else if (!texturesLoaded) {
                        // Fallback: draw a simple colored circle if texture not loaded
                        sf::CircleShape fallback(SQUARE_SIZE * 0.3f);
                        fallback.setPosition(sf::Vector2f(
                            static_cast<float>(col * SQUARE_SIZE + SQUARE_SIZE * 0.2f),
                            static_cast<float>(row * SQUARE_SIZE + SQUARE_SIZE * 0.2f)
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
    }

    Piece* getPieceAt(Position<int> pos) const {
        if (!pos.isValid()) return nullptr;
        return board[pos.getRow()][pos.getCol()];
    }

    PieceColor getCurrentTurn() const {
        return currentTurn;
    }

    int countPieces(PieceColor color) const {
        return std::count_if(pieces.begin(), pieces.end(),
            [color](const std::unique_ptr<Piece>& piece) {
                return piece->getColor() == color;
            });
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
