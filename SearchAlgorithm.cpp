#include "SearchAlgorithm.hpp"

void BFSWithVistedStatesCheck::printSolution(Board& goal,
        map<Board, Move>& lastMoveMaps)
{
    // find the previous move until this board is root
    vector<BoardAndMovePair> solution;
    solution.reserve(32);
    Board currentBoard = goal;

    // find until currentBoard is root, which doesn't have the previous move
    while(lastMoves.count(currentBoard) == 1)
    {
        auto it = lastMoves.find(currentBoard);

        solution.emplace_back(make_pair(currentBoard, it->second));
        currentBoard = currentBoard.backtrackTheBoard(it->second);
    }
    cout << currentBoard; // the root board
    for(auto rit = solution.rbegin(); rit != solution.rend(); ++rit)
    {
        cout << rit->second;
        cout << rit->first;
    }
}

void AStar::printSolution(Board& goal, map<Board, Move>& lastMoveMaps)
{
    // find the previous move until this board is root
    vector<BoardAndMovePair> solution;
    solution.reserve(32);
    Board currentBoard = goal;

    // find until currentBoard is root, which doesn't have the previous move
    while(lastMoves.count(currentBoard) == 1)
    {
        auto it = lastMoves.find(currentBoard);

        solution.emplace_back(make_pair(currentBoard, it->second));
        currentBoard = currentBoard.backtrackTheBoard(it->second);
    }
    cout << currentBoard; // the root board
    for(auto rit = solution.rbegin(); rit != solution.rend(); ++rit)
    {
        cout << rit->second;
        cout << rit->first;
    }
}
