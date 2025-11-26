#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "Piece.h"
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Pawn.h"
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

    // Private helper methods
    std::string getPieceKey(PieceType type, PieceColor color) const;
    bool loadTextures();
    void loadWoodTexture();
    bool simulateMove(Position<int> from, Position<int> to, PieceColor& opponentColor);
    bool wouldLeaveKingInCheck(Position<int> from, Position<int> to);

public:
    Board();
    ~Board();

    void assignTextures();
    void initializeBoard();
    void updateBoardArray();
    bool movePiece(Position<int> from, Position<int> to);
    bool promotePawn(Position<int> position, PieceType newType);
    void draw(sf::RenderWindow& window, const sf::Font& font, const std::vector<Position<int>>& validMoves = std::vector<Position<int>>()) const;
    std::vector<Position<int>> getValidMoves(Position<int> from) const;
    Position<int> findKing(PieceColor color) const;
    bool isSquareUnderAttack(Position<int> square, PieceColor attackingColor) const;
    bool isPlayerInCheck(PieceColor color) const;
    bool hasAnyValidMoves(PieceColor color) const;
    bool isPlayerInCheckmate(PieceColor color) const;
    bool isPlayerInStalemate(PieceColor color) const;
    bool hasInsufficientMaterial() const;
    bool isFiftyMoveRule() const;
    bool isThreefoldRepetition() const;
    std::string getPositionHash() const;
    int getMaterialScore(PieceColor color) const;
    Piece* getPieceAt(Position<int> pos) const;
    PieceColor getCurrentTurn() const;
    int countPieces(PieceColor color) const;
    
    typedef bool (*MoveValidator)(const Piece*, Position<int>, Piece* board[8][8]);
    bool validateMoveWithFunction(Position<int> from, Position<int> to, MoveValidator validator);
    
    const std::vector<std::unique_ptr<Piece>>& getPieces() const;
};
