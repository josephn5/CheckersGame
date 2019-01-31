#ifndef CHECKER_AI_H
#define CHECKER_AI_H

using namespace std;

#include <utility>
#include <vector>
#include <map>


typedef map <int, checkerPiece *> pieceMap;
typedef pair<int, checkerPiece *> piecePair;

class checkersAI
{
    private:
        const int piece_color = 2;

        vector<vector<int>> possible_single_paths; //each element is an array that contains possible piece movements
        vector<vector<int>> possible_jump_paths; //first int in that array is prev location, and last element is next location 
        
        //checks if checkerPiece belongs to player
        bool belongs_to(checkerPiece * p, int player);

        //return true if piece can simply move one space
        bool single_space(int old_pos, int new_pos, pieceMap & pMap);

        //add the vector containg two pos, for a single space movement
        void add_single_space_moves(int old_pos, int new_pos);

        //return true if the piece can perform a single jump from old to new
        bool check_single_jump(int old_pos, int new_pos, pieceMap & pMap);

        //return true if this king piece can perform a single jump from old to new
        bool check_king_single_jump(int old_pos, int new_pos, pieceMap & pMap);

        //add a vector<int> to possible_paths for that single piece as position piecePos
        void generatePaths(int old_pos, const vector<int> & previousPositions, pieceMap & pMap);

        //add a vector<int> to possible_paths for that single piece as position piecePos
        void generateKingPaths(int old_pos, const vector<int> & previousPositions, pieceMap & pMap);

        //if piece is a king or not, call proper generatePaths
        void updatePaths(int old_pos, int new_pos, const vector<int> & previousPositions, bool isKing, pieceMap & pMap);

        //calculates the possible paths to move a piece
        void calculate_paths(pieceMap & pMap);


    public:

        //return pair of ints where first value is prev location and second is next location
        //to be passed into is_valid and move_piece
        pair<int, int> getNextMove(pieceMap & pMap);

        checkersAI(); //default constructor



};


#endif //CHECKER_AI_H//