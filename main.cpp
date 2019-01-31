using namespace std;

#include <SFML/Graphics.hpp>
#include "checkerPiece.h"
#include "gameInterface.h"
#include <iostream>

int main()
{
	bool game_started = false;

	sf::Color c4(223, 226, 134); //yellow color for piece
	sf::Color c3(50, 35, 7); //brown color for piece
	sf::Color c2(250, 232, 70); //yellow color for board
	sf::Color c1(121, 92, 50); //brown color for board
	sf::Color bg_color(100, 100, 100); //grey background color

	gameSetup::checkersInterface checkerInterface(bg_color, c1, c2, c3, c4);

	checkerInterface.create_board();
	checkerInterface.fill_board();
	checkerInterface.create_pieces();

	while (checkerInterface.w.isOpen())
	{
		sf::Event evnt;

		while (checkerInterface.w.pollEvent(evnt))
		{

			if (evnt.type == evnt.Closed)
			{
				checkerInterface.w.close();
				break;
			}

			else if (evnt.type == evnt.MouseButtonReleased && evnt.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2i  mousePos = sf::Mouse::getPosition(checkerInterface.w);
				checkerInterface.handle_click(mousePos);
			}

		}

		checkerInterface.refresh_window();
	

	}
	
	return 0;
}