#include <vector>
#include <list>
#include <queue>
#include <set>
#include <iostream>
#include <utility>
#include "board.hpp"

using std::queue;
using std::cout;
using std::endl;
using std::set;
using std::move;
using std::list;

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
                if(b.canRedBlockUnblockInOneStep())
                {
                    cout << "find the solution!!" << endl;
                    exit(1);
                }
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
                if(b.canRedBlockUnblockInOneStep())
                {
                    cout << "find the solution" << endl;

                    // plus the last step
                    cout << "cost: " << cost(b) + 1 << endl;
                    return;
                }

                // can't unblock the red block
                // generate the available moves
                vector<Board> newStates = b.moves();
                for(auto e : newStates)
                {
                    if(visited.find(e) != visited.end())
                        continue;

                    // this state isn't visited;
                    // push to the queue
                    states.emplace_back(e);
                    visited.insert(e);
                }
                b = move(states.front());
                states.pop_front();
                if(currentDepth < b.getCost())
                {
                    currentDepth += 1;
                    cout << "depth += 1 => depth is: " << currentDepth << endl;
                    cout << "expanded_states: " << getExpandedStates() << endl;
                    cout << "size of queue: " << states.size() << endl;
                    
                    // if(currentDepth == 2) 
                    // {
                    //     Board* barr[3] {NULL, NULL, NULL}; 
                    //     int iterator = 0;
                    //     for(auto e : states)
                    //     {
                    //         if(e.getBlock(2).get(0) == 14 && e.getBlock(5).get(0) == 21)
                    //         {
                    //             barr[iterator] = &e;
                    //             iterator++;
                    //             assert(iterator == 0 || iterator == 1 || iterator == 2);
                    //             cout << "depth: " << currentDepth<< " ==>" << endl;
                    //             cout << e;
                    //         }
                    //     }
                    //     if(iterator == 1)
                    //     {
                    //         cout << "done!!" << endl;
                    //     }
                    //     else {
                    //         cout << "are two object the same??" << endl;
                    //         cout << (*barr[0] < *barr[1]) << endl;
                    //         cout << (*barr[1] < *barr[0]) << endl;
                    //     }
                    //     cout << "reach maximum depth, stop" << endl;
                    //     return;
                    // }
                    // cout << "--------------------" << endl;
                }
            }
            cout << "find the goal state: " << cost(b) << endl;
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
};
