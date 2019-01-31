#ifndef CHECKERPIECE_H
#define CHECKERPIECE_H

#include <SFML/Graphics.hpp>
#include <iostream>

class checkerPiece
{

	private:
		int square_location;
		int player;
		bool king;

	public:
		sf::CircleShape * circ;

		//constructor for checkerPiece, takes in which square on the board piece is in
		checkerPiece(int square = 0, int p = 1);

		//copy constructor for checkerPiece
		checkerPiece(const checkerPiece & c);

		//print function
		void print(ostream & out) const;

		//return the integer value of the square that the piece is in
		int location() const;

		//return player the piece belongs to, 1 is white, 2 is brown
		int get_player() const;

		//return true is the piece is a king
		bool is_king() const;

		//set the piece to be a king
		void set_king();

		//change the position of the checkerPiece
		void change_square(int new_square);

		//initialize piece color, outline, position
		static void initialize_piece(sf::Vector2f v, sf::Color c, checkerPiece * p);

		//destructor
		~checkerPiece();


};


ostream & operator << (ostream & out, const checkerPiece & c);

#endif //CHECKERPIECE_H