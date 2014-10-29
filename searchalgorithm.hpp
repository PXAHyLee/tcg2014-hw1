#pragma once

#include "common.h"
#include "board.hpp"

#include <list>
#include <queue>
#include <map>
#include <set>
#include <utility>
#include <cstdlib>

using std::priority_queue;
using std::queue;
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
        SearchAlgorithm(ostream& out) : os(out), expanded_states(0) { }
        virtual void solveProblem() = 0;
        virtual ~SearchAlgorithm() { }
    protected:
        /* evaluation functions: g and h => cost = g + h */
        /* g: current cost function of the path P */
        /* h: heuristic function of the path P */
        int currentCost(const Board& b) { return b.getCost(); }
        void increaseExpandedStatesByOne() { expanded_states++; }
        uint64_t getExpandedStates() const { return expanded_states; }
        virtual int heuristic(const Board& b) { return 0; }
        virtual int cost(const Board& b)
        {
            return currentCost(b) + heuristic(b);
        }
        virtual void printStatisticMesssage(Board& goal);
        virtual void printSolution(Board& goal, map<Board, Move>& lastMoveMaps);
        virtual size_t getContainerSize() = 0;

        /* data members */
        map<Board, Move> lastMoves;
        set<Board> visited;
        ostream& os;
    private:
        uint64_t expanded_states;
};

class BFSWithVistedStatesCheck final : public SearchAlgorithm
{
    public:
        BFSWithVistedStatesCheck(const Board root, ostream& os) : 
            SearchAlgorithm(os)
        {
            states.push_back(root);
            visited.insert(root);
        }
        void solveProblem() override;
        size_t getContainerSize() override { return states.size(); }
    private:
        list<Board> states;
};

class AStar final : public SearchAlgorithm
{
    public:
        AStar(const Board root, ostream& os) : SearchAlgorithm(os)
        {
            states.emplace(root);
            visited.emplace(root);
        }
        void solveProblem() override;
        size_t getContainerSize() override { return states.size(); }
    protected:
        int heuristic(const Board& b) override;
    private:
        // a > b: Board a is less promising than Board b
        priority_queue<Board, vector<Board>, greater<Board>> states;
};
