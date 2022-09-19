#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.h"

using namespace std;


struct Board {
    int row;
    int column;
    int numOfMines;
    int numOfMinesCopy;
    bool debugMode = false;
    bool gameActive = true;
    int numOfTilesRevealed = 0;

    vector<Tile> tileVector;
    vector<sf::Sprite> boardButtons;
    vector<sf::Sprite> digitsVector;

    Board();
    void LoadConfig(); // Loads the rows/columns/mines from the config file
    void LoadTest(string fileName); //Reads in the brd files
    void LeftClickTest(sf::Event &event, sf::Vector2i mousePosition);
    void LeftClickDebug(sf::Event &event, sf::Vector2i mousePosition, sf::RenderWindow &window);
    void LeftClickTile(sf::Event &event, sf::Vector2i mousePosition, sf::RenderWindow &window);
    void RevealMine(Tile &tile);
    void AdjacentMineUpdate();
    void ClearVectors();
    void LoadBoard();
    void ResetBoard(sf::Event &event, sf::Vector2i mousePosition);
    void DrawButtons(sf::RenderWindow &window);
    void DrawTiles(sf::RenderWindow &window);
    void RightClickFlag(sf::Event &event, sf::Vector2i mousePosition);
    void DrawFlagCounter(sf::RenderWindow &window);
    void DrawBoard(sf::RenderWindow &window);
    int SingleInt(int position);
    void RandomizeMine();
    void LoseGame();

};



