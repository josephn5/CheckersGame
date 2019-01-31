#ifndef CHECKER_GAMESTATE_H
#define CHECKER_GAMESTATE_H

using namespace std;

#include <map>
#include <stack>	
#include <vector>
#include <iostream>
#include <utility>
#include "checkerPiece.h"
#include "checkersAI.h"

#define SINGLEPLAYERMODE 0
#define TWOPLAYERMODE 1

typedef map <int, checkerPiece *> pieceMap;
typedef pair<int, checkerPiece *> piecePair;

class gameState
{
	private:
		int current_turn; //1 or 2 for player 1/2, white/brown
		int game_mode; // 0 for Single-player vs AI, 1 for Two-player,
		map<int, int> piece_count; //12 each at start of the game
		stack<int> to_remove; //stack of ints/squares to remove from pieces when a jump occurs
		vector<vector<int>> multiJumpPaths; // list of all possible paths a piece can make by jumping
		checkersAI * checkersBot; //AI class that will make moves for player 2

		//given the differnece in position of squares return 7 for left, 9 for right
		int get_direction(int pos_diff);

		//find the difference between two positions
		int find_pos_diff(int old_pos, int new_pos);

		//returns true if that checkerPiece belongs to that player
		bool belongs_to(checkerPiece * p, int player);

		//remove all pieces in the stack from play
		void remove_all();
		
		//clear the pieces that could possibly be removed in the stack, doesnt actually remove pieces from play
		void clear_stack();

		//add that int/square to the stack to possibly be removed
		void push_piece(int square);

	public:
		bool game_over; //true when brown or white pieces = 0

		//map that keeps track of which piece on which square
		//initialized in gameInterface by create_pieces()
		pieceMap pMap;

		//default constructor 
		gameState();

		//return 1 or 2 for current turn
		int get_current_turn();

		//check if an int is already in the path, if so then stop
		static bool inPath(int pos, const vector<int> & path);

		//set the game mode ONE TIME WHEN GAME IS STARTED
		void set_game_mode(int mode);

		//return 0 or 1 depending on mode
		int get_game_mode();

		//switch current_turn after a move is made
		void switch_turn();
		
		//remove the piece at the current square by setting its value
		//in the map to nullptr, nullptr at a square number represents
		//no piece on that square
		void remove_piece(int square);

		//return the pair of oldpos newpos to be used in gameInterface
		pair<int, int> getAiMove();

		//allow the ai to make its move and update the game state
		void update_state_single();

		//check if move is valid then update board and pieces
		//IF MODE IS SINGLE PLAYER, ALSO TAKES CARE OF AI MOVEMENT IN HERE
		void update_state_two(int new_pos);

		//update the game state if a succesful move was made
		void update_state(int new_pos);

		//takes in int pos and int[] which is two rows of total length 16 to check if pos in rows
		static bool isBrownSquareHelper(int pos, int rows[]);
		//return true if the square number is a valid brown square on the board
		//used with gameState::validPos to check for valid positioning
		static bool isBrownSquare(int pos);

		//check whether the square is in the valid range of the board
		static bool validPos(int pos);

		//determine whether the move is legal
		bool is_valid(int old_pos, int new_pos);

		//return true if white piece moves diagonally up
		static bool diagonal_white(int old_pos, int new_pos);

		//return true if brown piece moves diagonally down
		static bool diagonal_brown(int old_pos, int new_pos);

		bool single_space(int old_pos, int new_pos);

		//update game status after every move is made
		void update_gameover();

		//modify multiJumpPaths to have vectors of legal jumps in form vector(old_pos -> new_pos)
		//but take into account 4 directions 
		void generateKingPaths(int old_pos, int piece_color, const vector<int> & previousPositions);

		//modify multiJumpPaths to have vectors of legal jumps in form vector(old_pos -> new_pos)
		//taking 2 directions into account
		void generatePaths(int old_pos, int piece_color, const vector<int> & previousPositions);

		//add new vector of paths to multiJumpPaths
		void updatePaths(int old_pos, int new_pos, int piece_color,const vector<int> & previousPositions, bool isKing);

		//return true if the piece can single jump to new_pos
		bool check_single_jump(int old_pos, int new_pos, int piece_color);

		//return 7 or 9 if the king piece can single jump to new pos, number is spaces from old to new pos
		int king_single_jump(int old_pos, int new_pos, int piece_color);

		//return true if the piece can jump to the new_pos and remove the piece
		bool single_jump(int old_pos, int new_pos);

		//return true if the piece can jump to new_pos and remove the pieces
		bool multiple_jumps(int old_pos, int new_pos, int rows_moved);

		//return true is piece is moving/jumping correctly
		bool valid_jumps(int old_pos, int new_pos);
	
		//return 1 for white and 2 for brown if winner
		int get_winner();

		//destructor
		~gameState();

};


#endif //CHECKER_GAMESTATE_H//
