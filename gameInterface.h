#ifndef GAMESETUP_H
#define GAMESETUP_H

#include <SFML/Graphics.hpp>
#include "checkerPiece.h"
#include "checkerGamestate.h"
#include <map>

typedef map <int, checkerPiece *> pieceMap;

namespace gameSetup
{
	class checkersInterface
	{
	private:

		bool game_started;
		gameState * checkerGameState;
		sf::RectangleShape board_array[64]; //64, one for each square
		pieceMap pMap; //key is square number and element is pointer to checkerPiece
		checkerPiece * selected_piece;
		sf::Color bg_color;
		sf::Color s_color1, s_color2; //colors are for the squares
		sf::Color p_color1, p_color2; //colors are for the pieces
		sf::Font font;

		//return true is click is within the rectangle
		static bool is_within(sf::Vector2f rectPos, sf::Vector2f rectSize, sf::Vector2i mousePos);

		//calculate coordinates for where piece should go after moving
		sf::Vector2f calc_pos(int square);

	public:
		sf::RenderWindow w;

		//constructor for checkersInterface
		checkersInterface(sf::Color bg, sf::Color c1, sf::Color c2, sf::Color c3, sf::Color c4);

		//change game_started value to true
		void start_game();

		//draw the main menu where the user decides the mode
		void draw_welcome();
		void draw_option_text();
		void draw_option_buttons();

		//display whos turn it is
		void display_turn();
			
		//create the 64 squares of the board
		void create_board();

		//create the initial 24 pieces
		void create_pieces();

		//draw the squares on the board
		void draw_board();

		//fill the squares with color
		void fill_board();

		//draw the checker pieces on appropriate squares
		void draw_pieces();

		//clear, draw, and display for window
		void refresh_window();

		//handle clicks on the window appropriately
		void handle_click(sf::Vector2i mousePos);

		// carry out the turn of AI
		void handle_ai_turn();

		// carry out turn of user from mouse position and valid move checking
		void handle_user_turn(sf::Vector2i mousePos);

		//change the position of piece to where user clicked
		void move_piece(checkerPiece * c, sf::Vector2f newPos);

		//calculate which square the click is in
		int square_clicked(sf::Vector2i mousePos);

		//select piece to be moved and highlight the outline
		void select_piece(checkerPiece * c);

		//unselect piece and return piece outline to black
		void unselect_piece(checkerPiece * c);

		//after the game ends, display winner to screen
		void show_winner();

		//returns a new copy of the checkerPiece
		checkerPiece * new_copy(checkerPiece * c);

		//destructor 
		~checkersInterface();
	

	};
}



#endif //GAMESETUP_H//
