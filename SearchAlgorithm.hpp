#pragma once

#include <vector>
#include <list>
#include <queue>
#include <map>
#include <set>
#include <iostream>
#include <utility>
#include <cstdlib>
#include "board.hpp"

using std::priority_queue;
using std::queue;
using std::cout;
using std::endl;
using std::set;
using std::move;
using std::list;
using std::map;
using std::greater;
using std::pair;
using std::make_pair;

typedef pair<Board, Move> BoardAndMovePair;

class SearchAlgorithm
{
    public:
        SearchAlgorithm() : expanded_states(0) { }
        virtual void solveProblem() = 0;
        virtual ~SearchAlgorithm() { }
    protected:
        // g: current cost of P
        int currentCost(const Board& b) { return b.getCost(); }
        void increaseExpandedStatesByOne() { expanded_states++; }
        uint64_t getExpandedStates() const { return expanded_states; }
        // cost = g(P) (+ h(P))
        virtual int cost(const Board& b) = 0;
    private:
        uint64_t expanded_states;
};

class BFSWithVistedStatesCheck final : public SearchAlgorithm
{
    public:
        BFSWithVistedStatesCheck(const Board& root)
        {
            states.push_back(root);
            visited.insert(root);
        }

        void solveProblem() override
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
            cout << "Find the goal state, and it costs " << cost(b) <<
                " steps to take here" << endl;
            cout << "statistic: " << endl;
            cout << "expanded states: " << getExpandedStates() << endl;
            cout << "size of containers: " << states.size() << endl;
            printSolution(b, lastMoves);
        }
    protected:
        int cost(const Board& b) override
        {
            return currentCost(b);
        }
    private:
        void printSolution(Board& goal, map<Board, Move>& lastMoveMaps);
        list<Board> states;
        set<Board> visited;
        map<Board, Move> lastMoves;
};

class AStar final : public SearchAlgorithm
{
    public:
        AStar(const Board& root)
        {
            states.emplace(root);
            visited.emplace(root);
        }

        void solveProblem() override
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
            cout << "Find the goal state, and it costs " << cost(b) <<
                " steps to take here" << endl;
            cout << "statistic: " << endl;
            cout << "expanded states: " << getExpandedStates() << endl;
            cout << "size of containers: " << states.size() << endl;

            printSolution(b, lastMoves);
        }
    protected:
        int cost(const Board& b) override
        {
            return currentCost(b) + b.numberOfObstacle();
        }
    private:
        void printSolution(Board& goal, map<Board, Move>& lastMoveMaps);
        // a > b: Board a is less promising than Board b
        priority_queue<Board, vector<Board>, greater<Board>> states;
        set<Board> visited;
        map<Board, Move> lastMoves;
};
