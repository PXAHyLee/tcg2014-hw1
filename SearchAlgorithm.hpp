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

using std::queue;
using std::cout;
using std::endl;
using std::set;
using std::move;
using std::list;
using std::map;
using std::pair;
using std::make_pair;

typedef pair<Board, Move> BoardAndMovePair;

class SearchAlgorithm
{
    public:
        virtual void solveProblem() = 0;
        virtual ~SearchAlgorithm() { }
    protected:
        // g: current cost of P
        int currentCost(const Board& b)
        {
            return b.getCost();
        }

        void increaseExpandedStatesByOne()
        {
            expanded_states++;
        }

        uint64_t getExpandedStates() const
        {
            return expanded_states;
        }

        // cost = g(P) (+ h(P))
        virtual int cost(const Board& b) = 0;
    private:
        vector<Board> states;
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
                        continue;

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

            // find the previous move until this board is root
            vector<BoardAndMovePair> solution;
            solution.reserve(16);

            while(lastMoves.count(b) == 1)
            {
                auto it = lastMoves.find(b);

                solution.push_back(make_pair(b, it->second));
                b = b.backtrackTheBoard(it->second);
            }
            // print the root board, it doesn't have the previousMove
            cout << b;

            // print the non-root board
            for(auto rit = solution.rbegin(); rit != solution.rend(); ++rit)
            {
                cout << rit->second;
                cout << rit->first;
            }
        }
    protected:
        // there is no heuristic function in BFSWithVistedStatesCheck
        int cost(const Board& b) override
        {
            return currentCost(b);
        }
    private:
        list<Board> states;
        set<Board> visited;
        map<Board, Move> lastMoves;
};
