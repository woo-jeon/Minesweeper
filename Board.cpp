#include <SFML/Graphics.hpp>
#include "Board.h"
#include "TextureManager.h"
#include "Random.h"
#include <fstream>
#include <iostream>
using namespace std;

Board::Board() {
    LoadConfig();
    LoadBoard();

}

void Board::LoadConfig() {
    ifstream file("boards/config.cfg");
    file >> row;
    file >> column;
    file >> numOfMines;
    numOfMinesCopy = numOfMines;
}

void Board::LoadTest(string fileName) {
    //clear
    ClearVectors();
    debugMode = false;

    //Read in .brd data
    string mineLocation;
    string fileLine;
    ifstream file(fileName);
    while (!file.eof()) {
        getline(file, fileLine);
        mineLocation += fileLine;
    }

    //Set row/column based on test brd
    row = fileLine.size();
    column = mineLocation.size()/row;

    //Load board
    LoadBoard();

    //update mines
    numOfMines = 0;
    for (int i = 0; i < mineLocation.size(); ++i) {
        if (mineLocation.at(i) == '1') {
            tileVector.at(i).minePresent = true;
            ++numOfMines;
        } else {
            tileVector.at(i).minePresent = false;
        }
    }
    numOfMinesCopy = numOfMines;
    AdjacentMineUpdate();
}

void Board::LeftClickTest(sf::Event &event, sf::Vector2i mousePosition) {
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        if (boardButtons.at(2).getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
//            LoadTest("boards/testboard1.brd");
            LoadTest("boards/testboard1.brd");
        }
        if (boardButtons.at(3).getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
            LoadTest("boards/testboard2.brd");
        }
        if (boardButtons.at(4).getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
            LoadTest("boards/testboard3.brd");
        }
    }
}

void Board::LeftClickDebug(sf::Event &event, sf::Vector2i mousePosition, sf::RenderWindow &window) {
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        if (boardButtons.at(1).getGlobalBounds().contains(mousePosition.x, mousePosition.y) && debugMode == false) {
            for (int i = 0; i < row * column; i++) {
                if (tileVector.at(i).minePresent) {
                    tileVector.at(i).mineSprite.setScale(1,1);
                    window.draw(tileVector.at(i).mineSprite);
                }
            }
            debugMode = true;
        } else if (boardButtons.at(1).getGlobalBounds().contains(mousePosition.x, mousePosition.y) && debugMode == true) {
            for (int i = 0; i < row * column; i++) {
                    tileVector.at(i).mineSprite.setScale(0,0);
                }
            debugMode = false;
        }
    }
}

void Board::LeftClickTile(sf::Event &event, sf::Vector2i mousePosition, sf::RenderWindow &window) {
    //Not a mine & no adjacent mines
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        for (int i = 0; i < tileVector.size(); ++i) {
            if (tileVector.at(i).bottomTileSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                if (tileVector.at(i).minePresent == false && tileVector.at(i).adjacentMineNum == 0 && !tileVector.at(i).flagPresent && !tileVector.at(i).tileRevealed) {
                    RevealMine(tileVector.at(i));
                }
            }
        }
    }

    //Not a mine & has adjacent mine
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        for (int i = 0; i < tileVector.size(); ++i) {
            if (tileVector.at(i).bottomTileSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y) && tileVector.at(i).flagPresent == false) {
                if (tileVector.at(i).minePresent == false && tileVector.at(i).adjacentMineNum > 0 && !tileVector.at(i).tileRevealed) {
                    tileVector.at(i).bottomTileSprite.setTexture(TextureManager::GetTexture("tile_revealed"));
                    tileVector.at(i).numberSprite.setTexture(TextureManager::GetTexture("number_" + to_string(tileVector.at(i).adjacentMineNum)));
                    tileVector.at(i).tileRevealed = true;
                    ++numOfTilesRevealed;
                }
            }
        }
    }

    //Is a mine
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        for (int i = 0; i < tileVector.size(); ++i) {
            if (tileVector.at(i).bottomTileSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y) && tileVector.at(i).flagPresent == false) {
                if (tileVector.at(i).minePresent == true) {
                    LoseGame();
                }
            }
        }
    }

    //Checks for win condition
    if ((row*column)-numOfMinesCopy == numOfTilesRevealed) {
        boardButtons.at(0).setTexture(TextureManager::GetTexture("face_win"));
        for (int i = 0; i < tileVector.size(); ++i) {
            if (tileVector.at(i).minePresent) {
                tileVector.at(i).DrawFlag();
            }
        }
        for (int i = 0; i < row * column; i++) {
            tileVector.at(i).mineSprite.setScale(0,0);
        }
        numOfMines = 0;
        gameActive = false;
    }
}

void Board::RevealMine(Tile &tile) {
    tile.bottomTileSprite.setTexture(TextureManager::GetTexture("tile_revealed"));
    tile.tileRevealed = true;
    ++numOfTilesRevealed;

    for (int i = 0; i < tileVector.size(); ++i) {
            int xDiff = tile.mineSprite.getPosition().x - tileVector.at(i).mineSprite.getPosition().x;
            int yDiff = tile.mineSprite.getPosition().y - tileVector.at(i).mineSprite.getPosition().y;
            int dist = sqrt((xDiff * xDiff) + (yDiff * yDiff));
            if (dist < 46 && dist != 0) {
                //Another revealed tile
                if (tileVector.at(i).adjacentMineNum == 0 && !tileVector.at(i).flagPresent && !tileVector.at(i).tileRevealed) {
                    tileVector.at(i).bottomTileSprite.setTexture(TextureManager::GetTexture("tile_revealed"));
                    RevealMine(tileVector.at(i));
                }
                //number tile
                if (tileVector.at(i).adjacentMineNum > 0 && !tileVector.at(i).flagPresent && !tileVector.at(i).tileRevealed) {
                    tileVector.at(i).bottomTileSprite.setTexture(TextureManager::GetTexture("tile_revealed"));
                    tileVector.at(i).numberSprite.setTexture(TextureManager::GetTexture("number_" + to_string(tileVector.at(i).adjacentMineNum)));
                    tileVector.at(i).tileRevealed = true;
                    ++numOfTilesRevealed;
                }
            }
    }
}

void Board::AdjacentMineUpdate() {
    for (int i = 0; i < tileVector.size(); ++i) {
        tileVector.at(i).adjacentMineNum = 0;
    }

    for (int i = 0; i < tileVector.size(); ++i) {
        for (int j = 0; j < tileVector.size(); ++j) {
            //if current tile is not a mine and compared tile is a mine, calculate distance
            if(tileVector.at(i).minePresent == false && tileVector.at(j).minePresent == true) {
                int xDiff = tileVector.at(i).mineSprite.getPosition().x - tileVector.at(j).mineSprite.getPosition().x;
                int yDiff = tileVector.at(i).mineSprite.getPosition().y - tileVector.at(j).mineSprite.getPosition().y;
                int dist = sqrt((xDiff * xDiff) + (yDiff * yDiff));
                if (dist < 46 && dist != 0) {
                    ++tileVector.at(i).adjacentMineNum;
                }
            }
        }
    }
}

void Board::LoseGame() {
    for (int i = 0; i < tileVector.size(); ++i) {
        if (tileVector.at(i).minePresent == true) {
            tileVector.at(i).bottomTileSprite.setTexture(TextureManager::GetTexture("tile_revealed"));
            tileVector.at(i).mineSprite.setScale(1,1);
        }
    }
    boardButtons.at(0).setTexture(TextureManager::GetTexture("face_lose"));
    gameActive = false;
}

void Board::ClearVectors() {
    tileVector.clear();
    boardButtons.clear();
    digitsVector.clear();

}

void Board::LoadBoard() {
    numOfTilesRevealed = 0;
    gameActive = true;
    //Load Tiles
    for (int i = 0; i < column; ++i) {
        for (int j = 0; j < row; ++j) {
            Tile tile;
            tile.bottomTileSprite.setPosition(j * 32, i * 32);
            tile.flagSprite.setPosition(j * 32, i * 32);
            tile.mineSprite.setPosition(j * 32, i * 32);
            tile.numberSprite.setPosition(j * 32, i * 32);
            tile.mineSprite.setScale(0,0);
            tileVector.push_back(tile);
        }
    }
    RandomizeMine();
    AdjacentMineUpdate();

    //Load Smiley
    sf::Sprite smiley(TextureManager::GetTexture("face_happy"));
    smiley.setOrigin(smiley.getTexture()->getSize().x/2,0);
    smiley.setPosition(sf::Vector2f(row*16, column*32));
    boardButtons.push_back(smiley);

    //Load Debug
    sf::Sprite debug(TextureManager::GetTexture("debug"));
    debug.setOrigin(debug.getTexture()->getSize().x/2,0);
    debug.setPosition(sf::Vector2f(row*16+128, column*32));
    boardButtons.push_back(debug);
    debugMode = false;

    //Load Test 1,2,3
    sf::Sprite test1(TextureManager::GetTexture("test_1"));
    sf::Sprite test2(TextureManager::GetTexture("test_2"));
    sf::Sprite test3(TextureManager::GetTexture("test_3"));
    test1.setOrigin(test1.getTexture()->getSize().x/2,0);
    test2.setOrigin(test2.getTexture()->getSize().x/2,0);
    test3.setOrigin(test3.getTexture()->getSize().x/2,0);
    test1.setPosition(sf::Vector2f(row*16+192, column*32));
    test2.setPosition(sf::Vector2f(row*16+256, column*32));
    test3.setPosition(sf::Vector2f(row*16+320, column*32));
    boardButtons.push_back(test1);
    boardButtons.push_back(test2);
    boardButtons.push_back(test3);
}

void Board::ResetBoard(sf::Event &event, sf::Vector2i mousePosition) {
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        if (boardButtons.at(0).getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
            ClearVectors();
            LoadConfig();
            LoadBoard();
        }
    }
}

void Board::DrawButtons(sf::RenderWindow &window) {
    for (int i = 0; i < 5; ++i) {
        window.draw(boardButtons.at(i));
    }
}

void Board::DrawTiles(sf::RenderWindow &window) {
    for (int i = 0; i < row*column; i++) {

        window.draw(tileVector.at(i).bottomTileSprite);
        window.draw(tileVector.at(i).numberSprite);
        window.draw(tileVector.at(i).flagSprite);
        window.draw(tileVector.at(i).mineSprite);
    }
}

void Board::RightClickFlag(sf::Event &event, sf::Vector2i mousePosition) {
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right) {
        for (int i = 0; i < tileVector.size(); ++i) {
            if (tileVector.at(i).bottomTileSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y) &&
                !tileVector.at(i).flagPresent && !tileVector.at(i).tileRevealed) {
                tileVector.at(i).DrawFlag();
                --numOfMines;
            } else if (tileVector.at(i).bottomTileSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y) &&
                       tileVector.at(i).flagPresent) {
                tileVector.at(i).flagSprite.setScale(0, 0);
                tileVector.at(i).flagPresent = false;
                ++numOfMines;
            }
        }
    }
}

void Board::DrawFlagCounter(sf::RenderWindow &window) {
    sf::Sprite digit(TextureManager::GetTexture("digits"));
    for (int i = 0; i < 11; ++i) {
        digit.setTextureRect(sf::IntRect(i*21,0,21,32));
        digitsVector.push_back(digit);
    }

    if (numOfMines < 0) {
        digitsVector.at(10).setPosition(sf::Vector2f(0, column * 32));
        window.draw(digitsVector.at(10));
    }

    int numOfDigits = to_string(abs(numOfMines)).size();

    switch (numOfDigits) {
        case 1:
            digitsVector.at(0).setPosition(sf::Vector2f(21, column * 32));
            window.draw(digitsVector.at(0));
            digitsVector.at(0).setPosition(sf::Vector2f(42, column * 32));
            window.draw(digitsVector.at(0));
            digitsVector.at(SingleInt(0)).setPosition(sf::Vector2f(63, column*32));
            window.draw(digitsVector.at(SingleInt(0)));
            break;
        case 2:
            digitsVector.at(0).setPosition(sf::Vector2f(21, column * 32));
            window.draw(digitsVector.at(0));
            digitsVector.at(SingleInt(0)).setPosition(sf::Vector2f(42, column*32));
            window.draw(digitsVector.at(SingleInt(0)));
            digitsVector.at(SingleInt(1)).setPosition(sf::Vector2f(63, column*32));
            window.draw(digitsVector.at(SingleInt(1)));
            break;
        case 3:
            digitsVector.at(SingleInt(0)).setPosition(sf::Vector2f(21, column*32));
            window.draw(digitsVector.at(SingleInt(0)));
            digitsVector.at(SingleInt(1)).setPosition(sf::Vector2f(42, column*32));
            window.draw(digitsVector.at(SingleInt(1)));
            digitsVector.at(SingleInt(2)).setPosition(sf::Vector2f(63, column*32));
            window.draw(digitsVector.at(SingleInt(2)));
            break;
        default:
            digitsVector.at(9).setPosition(sf::Vector2f(21, column*32));
            window.draw(digitsVector.at(9));
            digitsVector.at(9).setPosition(sf::Vector2f(42, column*32));
            window.draw(digitsVector.at(9));
            digitsVector.at(9).setPosition(sf::Vector2f(63, column*32));
            window.draw(digitsVector.at(9));
    }
}

void Board::DrawBoard(sf::RenderWindow &window) {
    DrawTiles(window);
    DrawButtons(window);
    DrawFlagCounter(window);
}

int Board::SingleInt(int position) {
    int singleDigit;
    string singleDigitStr;
    singleDigitStr = to_string(abs(numOfMines)).at(position);
    singleDigit = stoi(singleDigitStr);
    return singleDigit;
}

void Board::RandomizeMine() {
    int mineCount = 0;
    while(numOfMines != mineCount) {
        int randomInt = Random::Int(0,tileVector.size()-1);
        if (tileVector.at(randomInt).minePresent == false) {
            tileVector.at(randomInt).minePresent = true;
            ++mineCount;
        }
    }
}

