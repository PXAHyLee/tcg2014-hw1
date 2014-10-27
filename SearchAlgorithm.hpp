#include <vector>
#include <queue>
#include <iostream>
#include "board.hpp"

using std::queue;
using std::cout;
using std::endl;

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

class BFS final : public SearchAlgorithm
{
    public:
        BFS(const Board& root)
        {
            states.push(root);
        }

        void solveProblem() override
        {
            Board b = states.front();
            states.pop();
            int currentDepth = 0;
            while(!b.isGoal())
            {
                increaseExpandedStatesByOne();
                vector<Board> newStates = b.moves();
                for(auto e : newStates)
                {
                    states.emplace(e);
                }
                b = states.front();
                states.pop();
                if(currentDepth < b.getCost())
                {
                    currentDepth += 1;
                    cout << "depth += 1 => depth is: " << currentDepth << endl;
                    cout << "expanded_states: " << getExpandedStates() << endl;
                    cout << "size of queue: " << states.size() << endl;
                    cout << "--------------------" << endl;
                }
            }
            cout << b << endl;
            cout << "===" << __PRETTY_FUNCTION__ << "===" << endl;
            cout << "expanded states: " << getExpandedStates() << endl;
            cout << "cost of solution: " << b.getCost() << endl;
        }
    protected:
        // there is no heuristic function in BFS
        int cost(const Board& b) override
        {
            return currentCost(b);
        }
    private:
        queue<Board> states;
};
