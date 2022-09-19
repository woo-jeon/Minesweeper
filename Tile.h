#pragma once
#include <SFML/Graphics.hpp>

struct Tile {
    sf::Sprite mineSprite;
    sf::Sprite bottomTileSprite;
    sf::Sprite flagSprite;
    sf::Sprite numberSprite;

    bool flagPresent = false;
    bool minePresent = false;
    bool tileRevealed = false;
    int adjacentMineNum;

    Tile();
    void DrawFlag();
};

