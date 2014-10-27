#pragma once

#include <string>
#include <cassert>
#include <iostream>
#include <bitset>
#include <vector>
#include <cstdint>
#include "block.hpp"

using std::bitset;
using std::vector;
using std::cout;
using std::ostream;
using std::endl;

class SearchAlgorithm;

class Board
{
    public:
        Board(const bitset<36> initialBoard, uint8_t numberOfBlk,
                const Block* blkArray) : board(initialBoard), cost(0)
        {
            blocks.reserve(static_cast<int>(numberOfBlk));
            for(uint8_t i = 0; i < numberOfBlk; ++i)
                blocks.push_back(blkArray[i]);
        }

        // enumerates all legal moves 
        vector<Board> moves() const;

        // input: a board index
        // output: the block index, if exists. return -1 if there's no block
        //         going over this board index.
        int getBlockIndex(int boardIndex) const;

        bitset<36> getBoard() const
        {
            return board;
        }

        Block getBlock(int blockIndex) const
        {
            return Block(blocks.at(blockIndex));
        }

        int getNumberOfBlocks() const
        {
            return blocks.size();
        }

        void increaseCost()
        {
            cost++;
        }

        uint32_t getCost() const
        {
            return cost;
        }

        // TODO: unit test
        void updateTheBoard(int blkIdx, int smallEdgeIndex)
        {
            Block& blk = blocks[blkIdx];
            for(int i = blk.get(0); i <= blk.get(1); i += blk.getMultiplier())
            {
                assert(board[i] == true);
                board[i] = false;
            }
            blk.setSmallEdge(smallEdgeIndex);
            for(int i = blk.get(0); i <= blk.get(1); i += blk.getMultiplier())
            {
                assert(board[i] == false);
                board[i] = true;
            }
        }

        /* ===== opertors ===== */
        // return the bit of input index
        bool operator[](uint32_t index) const
        {
            assert(index < 36);
            return board[index];
        }

        /* static member function */
        static void setAlgorithm(SearchAlgorithm* a)
        {
            algorithm = a;
        }

        bool isGoal()
        {
            return board[16] == true && board[17] == true && 
                blocks[getBlockIndex(16)].getColor() == BlockColor::RED;
        }

        friend ostream& operator<<(ostream& os, const Board& b);
    private:
        static SearchAlgorithm* algorithm;
        bitset<36> board;
        vector<Block> blocks;
        uint32_t cost;
};
