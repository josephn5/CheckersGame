using namespace std;
#include "checkerPiece.h"
#include <iostream>

checkerPiece :: checkerPiece(int square, int p)
	:circ (new sf::CircleShape(35, 60)), square_location(square), player(p), king(false)
	{
	}

checkerPiece :: checkerPiece(const checkerPiece & c)
	: circ(new sf::CircleShape(c.circ->getRadius(), c.circ->getPointCount())), square_location(c.location()),
		player(c.get_player()), king(c.is_king())

{
}

void checkerPiece :: print(ostream & out) const
{
	out << circ->getPosition().x << " " << circ->getPosition().y;
}

int checkerPiece:: location() const
{
	return square_location;
}

void checkerPiece :: change_square(int new_square)
{
	square_location = new_square;
}

int checkerPiece::get_player() const
{
	return player;
}

bool checkerPiece::is_king() const
{
	return king;
}

void checkerPiece::set_king()
{
	circ->setOutlineThickness(6);
	king = true;
}

void checkerPiece::initialize_piece(sf::Vector2f v, sf::Color c, checkerPiece * p)
{
	p->circ->setPosition(v.x, v.y);
	p->circ->setFillColor(c);
	p->circ->setOutlineColor(sf::Color::Black);
	p->circ->setOutlineThickness(3);
}

checkerPiece :: ~checkerPiece()
{
	delete circ;
}

ostream & operator << (ostream & out, const checkerPiece & c)
{
	c.print(out);
	return out;
}