#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdint>
#include "parser.hpp"
#include "board.hpp"
#include "SearchAlgorithm.hpp"
using std::cout;
using std::string;

namespace unit_test {
    void block_unit_test(const Board& board)
    {
        // b1
        Block b1 {BlockColor::RED, BlockDirection::HORIZONTAL, 2, 0, 0};
        b1.unit_test();
        for(int i = 0; i < 36; i++)
            if(b1.isBelongsToThisBlock(i)) {
                cout << "the board index i: " << i
                    << " is beloned to this block" << endl;
            }
        cout << b1 << endl;

        // b2
        Block b2 {BlockColor::BROWN, BlockDirection::VERTICAL, 3, 14, 1};
        b2.unit_test();
        for(int i = 0; i < 36; i++)
            if(b2.isBelongsToThisBlock(i)) {
                cout << "the board index i: " << i
                    << " is beloned to this block" << endl;
            }
        cout << b2 << endl;

        for(int i = 0; i < 36; ++i) {
            cout << "blk idx of board index: " << i
                << " " << board.getBlockIndex(i) << endl;
        }
        cout << "--------------------" << endl;
        for(int i = 0; i < board.getNumberOfBlocks(); ++i)
        {
            int twoIndices[2] {-1, -1};
            board.getBlock(i).moveDistance(board.getBoard(), twoIndices);
        }
    }

    // main function in unit_test
    int main() {
        Parser parser(string("input/input2"));
        Board board(parser.getBoardFromFile());

        // unit_test::block_unit_test(board);

        SearchAlgorithm* algo = new BFSWithVistedStatesCheck(board);

        Board::setAlgorithm(algo);
        cout << "initial board is like: " << endl;
        cout << board << endl;
        cout << "--------------------" << endl;

        // try one moves
        // vector<Board> bb = board.moves();
        // cout << bb.size() << endl;
        // for(auto e : bb)
        // {
        //     cout << e << endl;
        // }

        algo->solveProblem();
        return 0;
    }
}

int main() {
    return unit_test::main();
}
