#include <iostream>
#include "Game.h"

int main() {
    try {
        std::cout << "=================================\n";
        std::cout << "    CHESS GAME WITH SFML\n";
        std::cout << "  Professional Graphics with PNG Images\n";
        std::cout << "=================================\n";
        std::cout << "Controls:\n";
        std::cout << "- Click to select and move pieces\n";
        std::cout << "- Press 'S' to save game history\n";
        std::cout << "- Press 'R' to reset the game\n";
        std::cout << "- Press 'ESC' to exit\n";
        std::cout << "=================================\n";
        std::cout << "Pieces per side (32 total):\n";
        std::cout << "  1 King\n";
        std::cout << "  1 Queen\n";
        std::cout << "  2 Rooks\n";
        std::cout << "  2 Bishops\n";
        std::cout << "  2 Knights\n";
        std::cout << "  8 Pawns\n";
        std::cout << "=================================\n";
        std::cout << "High-quality PNG images!\n";
        std::cout << "Professional appearance!\n";
        std::cout << "=================================\n\n";

        Game game;
        game.run();

        std::cout << "\nThank you for playing!\n";
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
