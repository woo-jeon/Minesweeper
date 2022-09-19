#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
using std::unordered_map;
using std::string;

//Code from Prof Fox's SFML helper class video

class TextureManager {
    static unordered_map<string, sf::Texture> textures;
public:
    static void LoadTexture(string textureName);
    static sf::Texture& GetTexture(string textureName);
    static void Clear();
};
