#include <SFML/Graphics.hpp>
#include "Board.h"
#include "TextureManager.h"
using namespace std;

int main() {
    Board board;
    sf::RenderWindow window(sf::VideoMode(board.row*32, (board.column*32)+100), "Minesweeper");

    while (window.isOpen()) {
        window.clear(sf::Color(255,255,255));
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);


        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (board.gameActive) {
                //Functionality for right-clicking to add/remove flag
                board.RightClickFlag(event, mousePosition);

                //Functionality for Debug
                board.LeftClickDebug(event, mousePosition, window);

                //Functionality for left-clicking tiles
                board.LeftClickTile(event, mousePosition,window);
            }

            //Functionality for resetting board
            board.ResetBoard(event, mousePosition);

           //Functionality for Test boards
           board.LeftClickTest(event, mousePosition);
        }

        board.DrawBoard(window);
        window.display();
    }
    TextureManager::Clear();
    return 0;
}