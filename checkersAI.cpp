#include "checkerGamestate.h"
#include "gameInterface.h"
#include "checkerPiece.h"
#include "checkersAI.h"
#include <algorithm>
#include <random>

bool checkersAI::belongs_to(checkerPiece * p, int player)
{
	return (p->get_player() == player);
}

bool checkersAI::single_space(int old_pos, int new_pos, pieceMap & pMap)
{
	int rows_moved = abs(old_pos / 8 - new_pos / 8);
	if (pMap[old_pos] ==nullptr || pMap[new_pos] != nullptr || rows_moved != 1 || !gameState::isBrownSquare(old_pos) || !gameState::isBrownSquare(new_pos))
		return false;
	if (pMap[old_pos]->is_king())
		return gameState::diagonal_white(old_pos, new_pos) || gameState::diagonal_brown(old_pos, new_pos);
	else
		return gameState::diagonal_brown(old_pos, new_pos);
}

void checkersAI::add_single_space_moves(int old_pos, int new_pos)
{
	vector<int> result;
	result.push_back(old_pos);
	result.push_back(new_pos);
	possible_single_paths.push_back(result);
}

bool checkersAI::check_single_jump(int old_pos, int new_pos, pieceMap & pMap)
{
	int difference = 1;

	if (pMap[old_pos + difference * 14] == nullptr && pMap[old_pos + difference * 7] != nullptr && old_pos + difference * 14 == new_pos)
	{
		return !checkersAI::belongs_to(pMap[old_pos + difference * 7], piece_color);
	}
	else if (pMap[old_pos + difference * 18] == nullptr && pMap[old_pos + difference * 9] != nullptr && old_pos + difference * 18 == new_pos)
	{
		return !checkersAI::belongs_to(pMap[old_pos + difference * 9], piece_color);
	}
	else
	{
		return false;
	}
}

bool checkersAI::check_king_single_jump(int old_pos, int new_pos, pieceMap & pMap)
{
	int difference;
	if (old_pos - new_pos > 0)
		difference = -1;
	else
		difference = 1;

	if (pMap[old_pos + difference * 14] == nullptr && pMap[old_pos + difference * 7] != nullptr && old_pos + difference * 14 == new_pos
		&& !checkersAI::belongs_to(pMap[old_pos + difference * 7], piece_color))
	{
		return true;
	}
	else if (pMap[old_pos + difference * 18] == nullptr && pMap[old_pos + difference * 9] != nullptr && old_pos + difference * 18 == new_pos
		&& !checkersAI::belongs_to(pMap[old_pos + difference * 9], piece_color))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void checkersAI::updatePaths(int old_pos, int new_pos, const vector<int> & previousPositions, bool isKing, pieceMap & pMap)
{
	vector<int> newPath;
	for (int i = 0; i < previousPositions.size(); ++i)
	{
		newPath.push_back(previousPositions[i]);
	}
	newPath.push_back(old_pos);
	newPath.push_back(new_pos);
	possible_jump_paths.push_back(vector<int> (newPath));
	if (isKing)
		generateKingPaths(new_pos, newPath, pMap);
	else
		checkersAI::generatePaths(new_pos, newPath, pMap);
}

void checkersAI::generateKingPaths(int old_pos, const vector<int> & previousPositions, pieceMap & pMap)
{

	if (checkersAI::single_space(old_pos, old_pos + 9, pMap))
		checkersAI::add_single_space_moves(old_pos, old_pos + 9);
	
	if (checkersAI::single_space(old_pos, old_pos + 7, pMap))
		checkersAI::add_single_space_moves(old_pos, old_pos + 7);

	if (checkersAI::single_space(old_pos, old_pos - 9, pMap))
		checkersAI::add_single_space_moves(old_pos, old_pos - 9);
	
	if (checkersAI::single_space(old_pos, old_pos - 7, pMap))
		checkersAI::add_single_space_moves(old_pos, old_pos - 7);

	if (checkersAI::check_king_single_jump(old_pos, old_pos + 18, pMap) != 0)
	{
		int new_pos = old_pos + 18;
		if (gameState::validPos(new_pos) && !gameState::inPath(new_pos, previousPositions) && gameState::isBrownSquare(new_pos))
			checkersAI::updatePaths(old_pos, new_pos, previousPositions, true, pMap);
	}

	if (checkersAI::check_king_single_jump(old_pos, old_pos - 18, pMap) != 0)
	{
		int new_pos = old_pos - 18;
		if (gameState::validPos(new_pos) && !gameState::inPath(new_pos, previousPositions) && gameState::isBrownSquare(new_pos))
			checkersAI::updatePaths(old_pos, new_pos, previousPositions, true, pMap);

	}	
	if (checkersAI::check_king_single_jump(old_pos, old_pos + 14, pMap) != 0)
	{
		int new_pos = old_pos + 14;
		if (gameState::validPos(new_pos) && !gameState::inPath(new_pos, previousPositions) && gameState::isBrownSquare(new_pos))
			checkersAI::updatePaths(old_pos, new_pos, previousPositions, true, pMap);
	}	
	if (checkersAI::check_king_single_jump(old_pos, old_pos - 14, pMap) != 0)
	{
		int new_pos = old_pos - 14;
		if (gameState::validPos(new_pos) && !gameState::inPath(new_pos, previousPositions) && gameState::isBrownSquare(new_pos))
			checkersAI::updatePaths(old_pos, new_pos, previousPositions, true, pMap);
	}

}

void checkersAI::generatePaths(int old_pos, const vector<int> & previousPositions, pieceMap & pMap)
{
	int difference = 1;
	
	if (checkersAI::single_space(old_pos, old_pos+7, pMap))
		checkersAI::add_single_space_moves(old_pos, old_pos +7);
	
	if (checkersAI::single_space(old_pos, old_pos + 9, pMap))
		checkersAI::add_single_space_moves(old_pos, old_pos + 9);

	if (checkersAI::check_single_jump(old_pos, old_pos + difference * 18, pMap))
	{
		int new_pos = old_pos + difference * 18;
		if (gameState::validPos(new_pos) && gameState::isBrownSquare(new_pos))
			checkersAI::updatePaths(old_pos, new_pos, previousPositions, false, pMap);
	}
	if (checkersAI::check_single_jump(old_pos, old_pos + difference * 14, pMap))
	{
		int new_pos = old_pos + difference * 14;
		if (gameState::validPos(new_pos) && gameState::isBrownSquare(new_pos))
			checkersAI::updatePaths(old_pos, new_pos, previousPositions, false, pMap);
	}
}


void checkersAI :: calculate_paths(pieceMap & pMap)
{
    for (int i = 0; i < 64; ++i)
	{
		if(pMap[i] != nullptr && pMap[i]->get_player()==2)
		{
			if (pMap[i]->is_king())
				generateKingPaths(pMap[i]->location(), vector<int>(), pMap);
			else	
				generatePaths(pMap[i]->location(), vector<int>(), pMap);
		}
	}
}



pair<int, int> checkersAI::getNextMove(pieceMap & pMap)
{
    calculate_paths(pMap);

	if (possible_jump_paths.empty() && possible_single_paths.empty())
		return make_pair(-1, -1);
	

	vector<int> lastPath;
	if (!possible_jump_paths.empty())
		lastPath = possible_jump_paths.back();
	else 
	{
		//Only avaiable moves are single space moves so just pick a random one
		random_device rng;
		mt19937 urng(rng());
		shuffle(possible_single_paths.begin(), possible_single_paths.end(), rng);
		lastPath = possible_single_paths.back();
	}
	
    pair<int, int> p = make_pair(lastPath.front(), lastPath.back());

	//PRINT PATHS 
	/*
	for (int i = 0; i < possible_paths.size(); ++i)
	{
		for (int j = 0; j < possible_paths[i].size(); ++j)
		{
			cout << possible_paths[i][j] << "-";
		}
		cout << endl;
	}
	*/
	//END PRINT PATHS

	possible_single_paths.clear();
	possible_jump_paths.clear();
    return p;
}

checkersAI :: checkersAI()
{

}