using namespace std;

#include <iostream>
#include <map>
#include <windows.h>
#include "checkerGamestate.h"
#include "gameInterface.h"
#include "checkerPiece.h"


typedef map <int, checkerPiece *> pieceMap;
typedef pair<int, checkerPiece *> piecePair;


namespace gameSetup
{

	bool checkersInterface :: is_within(sf::Vector2f rectPos, sf::Vector2f rectSize, sf::Vector2i mousePos)
	{
		float x = (float)mousePos.x;
		float y = (float)mousePos.y;
		bool within_x = x >= rectPos.x && x <= rectPos.x + rectSize.x;
		bool within_y = y >= rectPos.y && y <= rectPos.y + rectSize.y;
		return within_x && within_y;
	}

	checkersInterface::checkersInterface(sf::Color bg, sf::Color c1, sf::Color c2, sf::Color c3, sf::Color c4)
		:w(sf::VideoMode(900, 700), "Checkers", sf::Style::Titlebar | sf::Style::Close),
		bg_color(bg), s_color1(c1), s_color2(c2), p_color1(c3), p_color2(c4),
		game_started(false), selected_piece(nullptr), checkerGameState(new gameState)
	{
		w.setFramerateLimit(30);
		if (!font.loadFromFile("LemonMilk.otf"))
			cerr << "Error loading the font from file" << endl;
	}

	void checkersInterface :: start_game()
	{
		game_started = true;
	}

	void checkersInterface::display_turn()
	{
		sf::Text t1;
		t1.setFont(font);
		string s = " TURN: Player " + to_string(checkerGameState->get_current_turn());
		t1.setString(s);
		t1.setCharacterSize(23);
		t1.setPosition(700, 100);
		w.draw(t1);
	}

	void checkersInterface :: draw_welcome()
	{
		sf::Text t1, t2;
		t1.setFont(font);
		t2.setFont(font);
		t1.setString("Welcome to the game of Checkers!");
		t2.setString("Choose an option below: ");
		t1.setCharacterSize(35);
		t2.setCharacterSize(20);
		t1.setPosition(110, 50);
		t2.setPosition(200, 225);
		w.draw(t1);
		w.draw(t2);
	}

	void checkersInterface :: draw_option_text()
	{
		sf::Text t1, t2;
		t1.setFont(font);
		t2.setFont(font);
		t1.setCharacterSize(20);
		t2.setCharacterSize(20);
		t1.setString("Single player against an AI");
		t2.setString("Two player on the same computer");
		t1.setPosition(300, 300);
		t2.setPosition(300, 400);
		w.draw(t1);
		w.draw(t2);
	}

	void checkersInterface :: draw_option_buttons()
	{
		sf::RectangleShape rect1(sf::Vector2f(500.0f, 60.0f));
		sf::RectangleShape rect2(sf::Vector2f(500.0f, 60.0f));
		rect1.setFillColor(sf::Color(100, 100, 190));
		rect2.setFillColor(sf::Color(100, 100, 190));
		rect1.setOutlineColor(sf::Color::Yellow);
		rect2.setOutlineColor(sf::Color::Yellow);
		rect1.setPosition(250, 280);
		rect2.setPosition(250, 380);
		w.draw(rect1);
		w.draw(rect2);
	}

	void checkersInterface :: create_board()
	{
		int count = 0;
		for (int i = 0; i < 8; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				sf::RectangleShape rect(sf::Vector2f(87.5f, 87.5f));
				board_array[count] = rect;
				board_array[count++].setPosition(87.5f * i, 87.5f * j);
			}
		}
	}

	void checkersInterface :: draw_board()
	{
		for (int i = 0; i < 64; ++i)
		{
			w.draw(board_array[i]);
		}
	}

	void checkersInterface :: fill_board()
	{
		int row_alternator = 0;
		for (int i = 0; i < 8; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				checkerGameState->pMap[i * 8 + j] = nullptr; //initially each brown square on the board is empty
				if ((row_alternator + j + 1) % 2 == 0)
					board_array[i * 8 + j].setFillColor(s_color1);
				else
					board_array[i * 8 + j].setFillColor(s_color2);
			}
			++row_alternator;
		}
	}

	void checkersInterface :: create_pieces()
	{
		for (int i = 0; i < 3; ++i) //brown pieces
		{
 			for (int j = 0; j < 4; ++j)
			{
				int s = ((i + 1) % 2) + i * 8 + 2*j; //square value of each brown piece
				checkerGameState->pMap[s] = new checkerPiece(s, 2);
				float x_cor = 175 * j + ((i + 1) % 2) * 87.5f + 10;
				float y_cor = 87.5f * i + 10;
				checkerPiece::initialize_piece(sf::Vector2f(x_cor, y_cor), p_color1, checkerGameState->pMap[s]);
			}
		}
		
		for (int i = 5; i < 8; ++i) //yellow pieces
		{
			for (int j = 0; j < 4; ++j)
			{
				int s = ((i + 1) % 2) + i * 8 + 2 * j; //square value of each white piece
				checkerGameState->pMap[s] = new checkerPiece(s, 1);
				float x_cor = 175 * j + ((i + 1) % 2) * 87.5f + 10;
				float y_cor = 87.5f * i + 10;
				checkerPiece::initialize_piece(sf::Vector2f(x_cor, y_cor), p_color2, checkerGameState->pMap[s]);
				
			}
			
		}
	}

	void checkersInterface :: draw_pieces()
	{
		for (pieceMap::iterator it = checkerGameState->pMap.begin(); it != checkerGameState->pMap.end(); ++it)
		{
			if (it->second != nullptr)
				w.draw(*it->second->circ);
		}
	}

	void checkersInterface :: refresh_window()
	{
		w.clear(bg_color);
		if (game_started)
		{
			checkersInterface::draw_board();
			checkersInterface::draw_pieces();
			checkersInterface::display_turn();
			if (checkerGameState->game_over == true)
				checkersInterface::show_winner();
		}	
		else
		{
			checkersInterface::draw_welcome();
			checkersInterface::draw_option_buttons();
			checkersInterface::draw_option_text();
		}
		w.display();
	}

	void checkersInterface :: handle_ai_turn()
	{
		pair<int, int> posPair = checkerGameState->getAiMove();
		if (posPair.first == -1) //bot cant make any more legal moves, auto player 1 win
			checkerGameState->game_over = true;
					
		if (checkerGameState->is_valid(posPair.first, posPair.second))
		{
			//add a small delay turn
			Sleep(50);
			checkersInterface::move_piece(checkerGameState->pMap[posPair.first], checkersInterface::calc_pos(posPair.second));
			checkerGameState->update_state(posPair.second);
		}
		
	}

	void checkersInterface :: handle_user_turn(sf::Vector2i mousePos)
	{
		int s = checkersInterface::square_clicked(mousePos);
		checkerPiece * p = checkerGameState->pMap.at(s);
		if (checkerGameState->game_over == true)
			; //game has finished, do nothing
		else if (p != nullptr && selected_piece == nullptr && p->get_player() == checkerGameState->get_current_turn())
				checkersInterface::select_piece(p);
		else if (selected_piece != nullptr)
		{
			if (checkerGameState->is_valid(selected_piece->location(), s))
			{	
				checkersInterface::move_piece(selected_piece, checkersInterface::calc_pos(checkersInterface::square_clicked(mousePos)));
				checkerGameState->update_state(s);
				unselect_piece(selected_piece);
				if (checkerGameState->get_game_mode() == SINGLEPLAYERMODE)
					handle_ai_turn();
			}
			else //user clicked an invalid move
				unselect_piece(selected_piece);
		}
	}

	void checkersInterface :: handle_click (sf::Vector2i mousePos)
	{
		if (game_started && mousePos.x < 700) //less than 700 makes sure the click is on the board
		{	
			if (checkerGameState->get_game_mode() == SINGLEPLAYERMODE)
			{
				if (checkerGameState->get_current_turn()==1) //only handle_click if its player 1 or white's turn
				{
					handle_user_turn(mousePos);
				}
			}
			else
				handle_user_turn(mousePos);
		}
		else
		{
			if (is_within(sf::Vector2f(250, 280), sf::Vector2f(500, 60), mousePos)) //clicked option 1
			{
				checkerGameState->set_game_mode(SINGLEPLAYERMODE);
				start_game();
			}
			else if (is_within(sf::Vector2f(250, 380), sf::Vector2f(500, 60), mousePos)) //clicked option 2
			{
				checkerGameState->set_game_mode(TWOPLAYERMODE);
				start_game();
			}
			
		}
	}

	int checkersInterface :: square_clicked(sf::Vector2i mousePos)
	{
		int x_value = static_cast<int>((float)mousePos.x / 87.5);
		int y_value = static_cast<int>((float)mousePos.y / 87.5) * 8;
		return x_value + y_value;
	}

	sf::Vector2f checkersInterface :: calc_pos(int square)
	{
		int row = square / 8;
		int col = square % 8; 
		float x_cor = 87.5f * col + 10;
		float y_cor = 87.5f * row + 10;
		return sf::Vector2f(x_cor, y_cor);

	}

	checkerPiece * checkersInterface::new_copy(checkerPiece * c)
	{
		return new checkerPiece(*c);
	}

	void checkersInterface :: move_piece(checkerPiece * c, sf::Vector2f newPos)
	{
		int old_square = c->location();
		sf::Vector2i v(newPos); //change Vector2f to Vector2i for squre_clicked function
		int new_square = checkersInterface::square_clicked(v);
		c->circ->setPosition(newPos);
		c->change_square(new_square);
		checkerGameState->pMap[new_square] = c;
		checkerGameState->pMap[old_square] = nullptr;
	}

	void checkersInterface :: select_piece(checkerPiece * c)
	{
		selected_piece = c;
		c->circ->setOutlineColor(sf::Color::Red);
	}

	void checkersInterface::unselect_piece(checkerPiece * c)
	{
		selected_piece = nullptr;
		c->circ->setOutlineColor(sf::Color::Black);
	}

	void checkersInterface::show_winner()
	{
		sf::Text t1;
		sf::Text t2;
		t1.setFont(font);
		t2.setFont(font);
		string s = "Player " + to_string(checkerGameState->get_winner());
		t1.setString(s);
		t2.setString("WINS");
		t1.setPosition(720, 400);
		t2.setPosition(720, 450);
		w.draw(t1);
		w.draw(t2);
	}

	checkersInterface :: ~checkersInterface()
	{
		for (pieceMap::iterator it = checkerGameState->pMap.begin(); it != checkerGameState->pMap.end(); ++it)
		{
			delete it->second;
		}

		delete checkerGameState;
	}

}