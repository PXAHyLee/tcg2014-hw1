#include "searchalgorithm.hpp"

void SearchAlgorithm::printStatisticMesssage(Board& goal)
{
    cout << "Find the goal state, and it costs " << cost(goal) <<
        " steps to take here" << endl;
    cout << "statistic: " << endl;
    cout << "expanded states: " << getExpandedStates() << endl;
    cout << "size of containers: " << getContainerSize() << endl;
    printSolution(goal, lastMoves);
}


void SearchAlgorithm::printSolution(Board& goal, map<Board, Move>& lastMoveMaps)
{
    // find the previous move until this board is root
    vector<BoardAndMovePair> solution;
    solution.reserve(32);
    Board currentBoard = goal;

    // find until currentBoard is root, which
    // doesn't have the previous move
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

void BFSWithVistedStatesCheck::solveProblem()
{
    Board b = move(states.front());
    states.pop_front();
    int currentDepth = 0;

    while(!b.isGoal())
    {
        increaseExpandedStatesByOne();

        // generate the available moves
        BoardsAndMoves newStates = b.moves();
        for(int i = 0; i < newStates.first.size(); ++i)
        {
            if(visited.find(newStates.first[i]) != visited.end())
            {
                continue;
            }
            // this state isn't visited;
            // push to the queue
            states.emplace_back((newStates.first)[i]);
            visited.insert((newStates.first)[i]);
            lastMoves.insert(make_pair(newStates.first[i],
                        newStates.second[i]));
        }
        b = move(states.front());
        states.pop_front();
        if(currentDepth < b.getCost())
        {
            currentDepth += 1;
        }
    }
    printStatisticMesssage(b);

}

void AStar::solveProblem()
{
    Board b = move(states.top());
    states.pop();

    cout << "cost of initial state: " << cost(b) << endl;

    while(!b.isGoal())
    {
        increaseExpandedStatesByOne();

        // generate the available moves
        BoardsAndMoves newStates = b.moves();
        for(int i = 0; i < newStates.first.size(); ++i)
        {
            if(visited.find(newStates.first[i]) != visited.end())
            {
                continue;
            }
            // this state isn't visited;
            // push to the queue
            states.emplace((newStates.first)[i]);
            visited.insert((newStates.first)[i]);
            lastMoves.insert(make_pair(newStates.first[i],
                        newStates.second[i]));
        }
        b = move(states.top());
        states.pop();
    }
    printStatisticMesssage(b);
}

int AStar::heuristic(const Board& b)
{
    return b.numberOfObstacle();
}
