#include "Tile.h"
#include "TextureManager.h"
#include <SFML/Graphics.hpp>

Tile::Tile() {
    mineSprite.setTexture(TextureManager::GetTexture("mine"));
    bottomTileSprite.setTexture(TextureManager::GetTexture("tile_hidden"));
    flagSprite.setTexture(TextureManager::GetTexture("flag"));
    flagSprite.setScale(0,0);
}

void Tile::DrawFlag() {
    flagSprite.setScale(1, 1);
    flagPresent = true;
}
