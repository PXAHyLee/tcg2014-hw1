#include "board.hpp"
#include <iostream>

using std::cout;
using std::endl;

SearchAlgorithm* Board::algorithm = 0;
vector<Board> Board::moves() const
{
    /* TODO: to be implemented */
    vector<Board> boards;
    int numberOfLegalMoves {0};

    boards.reserve(4);
    for(int i = 0; i < blocks.size(); i++)
    {
        int twoDirection[2] {-1, -1};

        blocks[i].moveDistance(board, twoDirection);
        assert(twoDirection[0] >= 0 && twoDirection[1] >= 0);
        // cout << twoDirection[0] << " " << twoDirection[1] << endl;
        for(int j = twoDirection[0]; j != blocks[i].get(0); j += blocks[i].getMultiplier())
        {
            // c++11; test for copy constructor
            boards.push_back(Board(*this));
            //boards.emplace_back(*this);
            boards.back().updateTheBoard(i, j);
            boards.back().increaseCost();
            numberOfLegalMoves++;
        }

        for(int j = twoDirection[1]; j != blocks[i].get(1); j -= blocks[i].getMultiplier())
        {
            // cout << blocks[i] << endl;
            // cout << j << " " << (int)blocks[i].get(0) << " " << (int)blocks[i].get(1) << endl;
            // cout << "==========" << endl;
            // c++11; test for copy constructor
            boards.push_back(Board(*this));
            //boards.emplace_back(*this);
            boards.back().updateTheBoard(i,
                    j - (blocks[i].size() - 1) * blocks[i].getMultiplier());
            boards.back().increaseCost();
            numberOfLegalMoves++;
        }
    }
    // cout << __PRETTY_FUNCTION__ << ", numberOfLegalMoves: " <<
    //     numberOfLegalMoves << endl;
    return boards;
}

// a friend function isn't a member function of Board!
ostream& operator<<(ostream& os, const Board& b)
{
    auto tmpBoard = b.getBoard();
    for(int i = 0; i < 6; i++) {
        os << tmpBoard[0] << tmpBoard[1] <<
            tmpBoard[2] << tmpBoard[3] <<
            tmpBoard[4] << tmpBoard[5] << endl;
        tmpBoard >>= 6;
    }
    return os;
}

int Board::getBlockIndex(int boardIndex) const
{
    if(board[boardIndex] == false)
        return -1;
    for(int i = 0; i < blocks.size(); ++i)
    {
        if(blocks[i].isBelongsToThisBlock(boardIndex))
            return i;
    }
    // this shouldn't be reached
    // assert(true == false);
    return -1;
}
