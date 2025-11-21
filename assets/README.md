# Assets Directory

This directory is for game assets like sprites, sounds, and fonts.

## Structure

```
assets/
??? sprites/          # Chess piece images
?   ??? white_king.png
?   ??? white_queen.png
?   ??? white_rook.png
?   ??? white_bishop.png
?   ??? white_knight.png
?   ??? white_pawn.png
?   ??? black_king.png
?   ??? black_queen.png
?   ??? black_rook.png
?   ??? black_bishop.png
?   ??? black_knight.png
?   ??? black_pawn.png
??? sounds/           # Sound effects
?   ??? move.wav
?   ??? capture.wav
?   ??? check.wav
?   ??? checkmate.wav
??? fonts/            # Fonts for text
?   ??? Arial.ttf
??? screenshots/      # Screenshots for README
    ??? gameplay.png
    ??? menu.png
```

## Adding Assets

To use assets in the game:

1. **Sprites**: Place PNG files in `sprites/` directory
2. **Sounds**: Place WAV files in `sounds/` directory
3. **Fonts**: Place TTF files in `fonts/` directory

## Loading Assets

Example code to load a sprite:

```cpp
sf::Texture texture;
if (!texture.loadFromFile("assets/sprites/white_king.png")) {
    std::cerr << "Failed to load texture!\n";
}
```

## Free Asset Resources

- Chess pieces: [Wikimedia Commons](https://commons.wikimedia.org/wiki/Category:SVG_chess_pieces)
- Sounds: [Freesound](https://freesound.org/)
- Fonts: [Google Fonts](https://fonts.google.com/)

## License

Make sure any assets you add are properly licensed for use in this project.
