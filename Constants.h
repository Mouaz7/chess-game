#pragma once

namespace ChessConstants {
    // Board dimensions
    constexpr int BOARD_SIZE = 8;
    constexpr int SQUARE_SIZE = 100;
    constexpr int BORDER_SIZE = 40;
    constexpr int WINDOW_WIDTH = BORDER_SIZE * 2 + SQUARE_SIZE * BOARD_SIZE;
    constexpr int WINDOW_HEIGHT = BORDER_SIZE * 2 + SQUARE_SIZE * BOARD_SIZE;
    
    // Game rules
    constexpr int HALFMOVE_DRAW_LIMIT = 100;  // 50-move rule (100 halfmoves)
    constexpr int REPETITION_DRAW_LIMIT = 3;   // Threefold repetition
    
    // Material values (for evaluation)
    constexpr int PAWN_VALUE = 1;
    constexpr int KNIGHT_VALUE = 3;
    constexpr int BISHOP_VALUE = 3;
    constexpr int ROOK_VALUE = 5;
    constexpr int QUEEN_VALUE = 9;
    
    // UI rendering constants
    constexpr float MOVE_HINT_DOT_RADIUS = 0.15f;      // Radius for empty square hints
    constexpr float MOVE_HINT_RING_RADIUS = 0.45f;     // Radius for capture hints
    constexpr float MOVE_HINT_RING_THICKNESS = 4.0f;   // Border thickness for capture ring
    
    // Colors (RGB)
    namespace Colors {
        constexpr int LIGHT_SQUARE_R = 240;
        constexpr int LIGHT_SQUARE_G = 217;
        constexpr int LIGHT_SQUARE_B = 181;
        
        constexpr int DARK_SQUARE_R = 181;
        constexpr int DARK_SQUARE_G = 136;
        constexpr int DARK_SQUARE_B = 99;
        
        constexpr int SELECTED_HIGHLIGHT_R = 130;
        constexpr int SELECTED_HIGHLIGHT_G = 151;
        constexpr int SELECTED_HIGHLIGHT_B = 105;
        constexpr int SELECTED_HIGHLIGHT_A = 200;
        
        constexpr int CHECK_HIGHLIGHT_R = 220;
        constexpr int CHECK_HIGHLIGHT_G = 50;
        constexpr int CHECK_HIGHLIGHT_B = 50;
        constexpr int CHECK_HIGHLIGHT_A = 180;
    }
    
    // Asset paths
    namespace Assets {
        constexpr const char* BOARD_TEXTURE = "assets/board.jpg";
        constexpr const char* FONT_PATH = "C:\\Windows\\Fonts\\arial.ttf";
        constexpr const char* HISTORY_FILE = "game_history.txt";
        constexpr const char* RESULTS_FILE = "match_results.txt";
    }
}
