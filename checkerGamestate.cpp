#include "checkerGamestate.h"
#include "checkerPiece.h"
#include "checkersAI.h"
#include "checkersAI.cpp"
#include <cmath>

bool gameState::inPath(int pos, const vector<int> & path)
{
	int size = path.size();
	for (int i = 0; i < size; ++i)
	{
		if (path[i] == pos)
			return true;
	}
	return false;
}

int gameState::get_direction(int pos_diff)
{
	if (pos_diff % 7 == 0)
		return 7;
	else
		return 9;
}

int gameState::find_pos_diff(int old_pos, int new_pos)
{
	if (current_turn == 1)
		return old_pos - new_pos;
	else
		return new_pos - old_pos;
}

bool gameState::belongs_to(checkerPiece * p, int player)
{
	return (p->get_player() == player);
}

void gameState::remove_all()
{
	while (!to_remove.empty())
	{
		int j = to_remove.top();
		to_remove.pop();
		gameState::remove_piece(j);
	}
}

void gameState::clear_stack()
{
	while (!to_remove.empty())
	{
		to_remove.pop();
	}
}

void gameState::push_piece(int square)
{
	to_remove.push(square);
}

gameState :: gameState()
	:current_turn(1), game_over(false)
{
	piece_count[1] = 12;
	piece_count[2] = 12;
}

int gameState::get_current_turn()
{
	return current_turn;
}

void gameState::set_game_mode(int mode)
{
	game_mode = mode;
	checkersBot = new checkersAI();
}

int gameState::get_game_mode()
{
	return game_mode;
}

void gameState::switch_turn()
{
	current_turn = (current_turn % 2) + 1;
}

void gameState::remove_piece(int square)
{
	--piece_count[pMap[square]->get_player()];
	delete pMap[square];
	pMap[square] = nullptr;
}

void gameState::update_state_single()
{

}

pair<int, int> gameState::getAiMove()
{
	return checkersBot->getNextMove(pMap);
}

void gameState::update_state_two(int new_pos)
{
	if (new_pos / 8 == 0 || new_pos/8 == 7)
		pMap[new_pos]->set_king();

	gameState::update_gameover();
 	gameState::switch_turn();
}

void gameState :: update_state(int new_pos)
{
	if (new_pos / 8 == 0 || new_pos/8 == 7)
		pMap[new_pos]->set_king();

	gameState::update_gameover();
 	gameState::switch_turn();


}

bool gameState :: isBrownSquareHelper(int pos, int rows[])
{		
	for (int i = 0; i < 8; ++i) //8 BECAUSE TWO ROWS AND EACH ROW HAS 4 BROWN SQUARES
	{
		if (rows[i] == pos)
			return true;
	}
	return false;
}	

bool gameState :: isBrownSquare(int pos)
{

	if (pos < 16) //first two rows etc
	{
		int rows[8] = {1, 3, 5, 7, 8, 10, 12, 14}; //brown squares in the first row
		return gameState::isBrownSquareHelper(pos, rows);
	}
	else if (pos < 32)
	{
		int rows[8] = {17, 19, 21, 23, 24, 26, 28, 30};
		return gameState::isBrownSquareHelper(pos, rows);
	}
	else if (pos < 48)
	{
		int rows[8] = {33, 35, 37, 39, 40, 42, 44, 46};
		return gameState::isBrownSquareHelper(pos, rows);
	}
	else
	{
		int rows[8] = {49, 51, 53, 55, 56, 58, 60, 62};
		return gameState::isBrownSquareHelper(pos, rows);
	}

}

bool gameState::validPos(int pos)
{
	return (pos >= 0 && pos <= 63);
}

bool gameState :: is_valid(int old_pos, int new_pos)
{
	if (pMap[old_pos] == nullptr || pMap[new_pos] != nullptr ||pMap[old_pos]->get_player() != current_turn 
							|| !isBrownSquare(old_pos) || !isBrownSquare(new_pos)) //clicked an empty square or 
		return false;															//piece that doesnt belong to player 
	

	return gameState::valid_jumps(old_pos, new_pos);
}

bool gameState::single_space(int old_pos, int new_pos)
{
	if (pMap[old_pos]->is_king())
		return diagonal_white(old_pos, new_pos) || diagonal_brown(old_pos, new_pos);
	if (current_turn == 1)
		return diagonal_white(old_pos, new_pos);
	else
		return diagonal_brown(old_pos, new_pos);
}

bool gameState :: diagonal_brown(int old_pos, int new_pos)
{
	int pos_diff = new_pos - old_pos;
	bool right_down = pos_diff % 9 == 0;
	bool left_down = pos_diff % 7 == 0;
	return (pos_diff > 0) && (right_down || left_down);
}

bool gameState :: diagonal_white(int old_pos, int new_pos)
{
	int pos_diff = old_pos - new_pos;
	bool left_up = pos_diff % 7 == 0;
	bool right_up = pos_diff % 9 == 0;
	return (pos_diff > 0) && (left_up || right_up);
}

void gameState::update_gameover()
{
	game_over = (piece_count[1] == 0) || (piece_count[2] == 0);
}

bool gameState::check_single_jump(int old_pos, int new_pos, int piece_color)
{
	int difference;
	if (piece_color == 1)
		difference = -1;
	else
		difference = 1;
	if (pMap[old_pos + difference * 14] == nullptr && pMap[old_pos + difference * 7] != nullptr && old_pos + difference * 14 == new_pos)
	{
		return !gameState::belongs_to(pMap[old_pos + difference * 7], piece_color);
	}
	else if (pMap[old_pos + difference * 18] == nullptr && pMap[old_pos + difference * 9] != nullptr && old_pos + difference * 18 == new_pos)
	{
		return !gameState::belongs_to(pMap[old_pos + difference * 9], piece_color);
	}
	else
	{
		return false;
	}
}

int gameState::king_single_jump(int old_pos, int new_pos, int piece_color)
{
	int difference;
	if (old_pos - new_pos > 0)
		difference = -1;
	else
		difference = 1;
	if (pMap[old_pos + difference * 14] == nullptr && pMap[old_pos + difference * 7] != nullptr && old_pos + difference * 14 == new_pos
		&& !gameState::belongs_to(pMap[old_pos + difference * 7], piece_color))
	{
		return difference * 7;
	}
	else if (pMap[old_pos + difference * 18] == nullptr && pMap[old_pos + difference * 9] != nullptr && old_pos + difference * 18 == new_pos
		&& !gameState::belongs_to(pMap[old_pos + difference * 9], piece_color))
	{
		return difference * 9;
	}
	else
	{
		return 0;
	}
}

bool gameState::single_jump(int old_pos, int new_pos)
{
	int difference, direction;
	bool k = pMap[old_pos]->is_king();
	int piece_color = pMap[old_pos]->get_player();

	if (old_pos - new_pos > 0)
		difference = -1;
	else
		difference = 1;

	if (k)
	{
		if (new_pos == (old_pos + difference * 18) )
			direction = gameState::king_single_jump(old_pos, old_pos + difference * 18, piece_color);
		else if (new_pos == (old_pos + difference * 14) )
			direction = gameState::king_single_jump(old_pos, old_pos + difference * 14, piece_color);

		if (direction != 0)
		{
			gameState::push_piece(old_pos + direction);
			old_pos = old_pos + direction * 2;
		}
	}
	else if (gameState::check_single_jump(old_pos, old_pos + difference * 14, piece_color) && 
									new_pos == old_pos + difference * 14 &&
									pMap[old_pos+difference*7]->get_player() != piece_color)
	{
		gameState::push_piece(old_pos + difference * 7);
		old_pos = old_pos + difference * 14;
	}
	else if (gameState::check_single_jump(old_pos, old_pos + difference * 18, piece_color) && 
										new_pos == old_pos + difference * 18 &&
										pMap[old_pos + difference * 9]->get_player() != piece_color)
	{
		gameState::push_piece(old_pos + difference * 9);
		old_pos = old_pos + difference * 18;
	}
	else
	{
		gameState::clear_stack();
		return false;
	}

	if (old_pos == new_pos)
		gameState::remove_all();
	else
		gameState::clear_stack();

	return old_pos == new_pos;
	
}

void gameState::updatePaths(int old_pos, int new_pos, int piece_color, const vector<int> & previousPositions, bool isKing)
{
	vector<int> newPath;
	for (int i = 0; i < previousPositions.size(); ++i)
	{
		newPath.push_back(previousPositions[i]);
	}
	newPath.push_back(old_pos);
	newPath.push_back(new_pos);
	multiJumpPaths.push_back(vector<int> (newPath));
	if (isKing)
		generateKingPaths(new_pos, piece_color, newPath);
	else
		generatePaths(new_pos, piece_color, newPath);
}

void gameState::generatePaths(int old_pos, int piece_color, const vector<int> & previousPositions)
{
	int difference;
	if (piece_color == 1)
		difference = -1;
	else
		difference = 1;
	
	if (gameState::check_single_jump(old_pos, old_pos + difference * 18, piece_color))
	{
		int new_pos = old_pos + difference * 18;
		if (validPos(new_pos))
			gameState::updatePaths(old_pos, new_pos, piece_color, previousPositions, false);
	}
	if (gameState::check_single_jump(old_pos, old_pos + difference * 14, piece_color))
	{
		int new_pos = old_pos + difference * 14;
		if (validPos(new_pos))
			gameState::updatePaths(old_pos, new_pos, piece_color, previousPositions, false);
	}
}

void gameState::generateKingPaths(int old_pos, int piece_color, const vector<int> & previousPositions)
{
	if (gameState::king_single_jump(old_pos, old_pos + 18, piece_color) != 0)
	{
		int new_pos = old_pos + 18;
		if (validPos(new_pos) && !inPath(new_pos, previousPositions))
			gameState::updatePaths(old_pos, new_pos, piece_color, previousPositions, true);
	}

	if (gameState::king_single_jump(old_pos, old_pos - 18, piece_color) != 0)
	{
		int new_pos = old_pos - 18;
		if (validPos(new_pos) && !inPath(new_pos, previousPositions))
			gameState::updatePaths(old_pos, new_pos, piece_color, previousPositions, true);

	}	
	if (gameState::king_single_jump(old_pos, old_pos + 14, piece_color) != 0)
	{
		int new_pos = old_pos + 14;
		if (validPos(new_pos) && !inPath(new_pos, previousPositions))
			gameState::updatePaths(old_pos, new_pos, piece_color, previousPositions, true);
	}	
	if (gameState::king_single_jump(old_pos, old_pos - 14, piece_color) != 0)
	{
		int new_pos = old_pos - 14;
		if (validPos(new_pos) && !inPath(new_pos, previousPositions))
			gameState::updatePaths(old_pos, new_pos, piece_color, previousPositions, true);
	}

}

bool gameState::multiple_jumps(int old_pos, int new_pos, int rows_moved)
{
	int difference, direction;
	bool k = pMap[old_pos]->is_king();
	int piece_color = pMap[old_pos]->get_player();
	if (old_pos - new_pos > 0)
		difference = -1;
	else
		difference = 1;

	if (k)
		generateKingPaths(old_pos, piece_color, vector<int>()); //Pass an empty vector because this is the start of path search
	else
		generatePaths(old_pos, piece_color, vector<int>()); //Pass an empty vector because this is the start of path search

	for (int i = 0; i < multiJumpPaths.size(); ++i)
	{
		if (multiJumpPaths[i].back() == new_pos)
		{
			int pathSz = multiJumpPaths[i].size();
			for (int j = 0; j < pathSz; j  = j + 2)
			{
				int curPos = multiJumpPaths[i][j];
				int nxtPos = multiJumpPaths[i][j+1];
				int squareToRemove = curPos - ((curPos-nxtPos) * 0.5);
				gameState::push_piece(squareToRemove);
			}
			old_pos = new_pos;
			break;
		}
	}

	multiJumpPaths.clear();

	//If old_pos == new_pos then move is valid and pieces may need to be removed
	//Else reset the stack that would have removed pieces
	if (old_pos == new_pos)
		gameState::remove_all();
	else
		gameState::clear_stack();
	return old_pos == new_pos;
}

bool gameState::valid_jumps(int old_pos, int new_pos)
{
	int rows_moved = abs(old_pos / 8 - new_pos / 8);

	 if (rows_moved == 1)
		return gameState::single_space(old_pos, new_pos);
	else if (rows_moved == 2)
		return gameState::single_jump(old_pos, new_pos);
	else if (rows_moved == 0 || rows_moved >= 4 && rows_moved % 2 == 0)
		return gameState::multiple_jumps(old_pos, new_pos, rows_moved);
	else
		return false;
}

int gameState::get_winner()
{
	if (piece_count[1] == 0)
		return 2;
	else if (piece_count[2] == 0)
		return 1;
	else
		return 1;
}

gameState :: ~gameState()
{
	if (checkersBot != nullptr)
		delete checkersBot;
}