rule:
	g++ -std=c++11 -o project3.out main.cpp Board.cpp Tile.cpp Random.cpp TextureManager.cpp -I/usr/local/Cellar/sfml/2.5.1_1/include -L/usr/local/Cellar/sfml/2.5.1_1/lib -lsfml-graphics -lsfml-window -lsfml-system
