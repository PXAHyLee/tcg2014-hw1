#pragma once

#include <cstring>
#include <string>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <bitset>
#include <vector>
#include <list>
#include <utility>
#include "move.hpp"
#include "block.hpp"

using std::bitset;
using std::vector;
using std::list;
using std::cout;
using std::pair;
using std::ostream;
using std::endl;

class SearchAlgorithm;
class Board;
typedef pair<vector<Board>, vector<Move>> BoardsAndMoves;

class Board
{
    public:
        // [reminder] root has undefined previous move
        Board(const bitset<36> initialBoard, uint8_t numberOfBlk,
                const Block* blkArray) : cost(0), board(initialBoard)
        {
            // initialize to zero
            memset(hashes, 0, sizeof(hashes));
            blocks.reserve(static_cast<int>(numberOfBlk));
            for(uint8_t i = 0; i < numberOfBlk; ++i)
            {
                blocks.push_back(blkArray[i]);
                hashes[i] = blocks[i].hash();
            }
        }

        // enumerates all legal moves 
        BoardsAndMoves moves();

        // input: a board index
        // output: the block index, if exists. return -1 if there's no block
        //         going over this board index.
        int getBlockIndex(int boardIndex) const;

        bitset<36> getBoard() const { return board; }

        Block getBlock(int blockIndex) const
        {
            return Block(blocks.at(blockIndex));
        }

        int getNumberOfBlocks() const { return blocks.size(); }

        void increaseCost() { cost++; }

        uint32_t getCost() const { return cost; }

        void updateTheBoard(int blkIdx, int smallEdgeIndex)
        {
            Block& blk = blocks[blkIdx];
            int org_hash = blk.hash();
            for(int i = blk.get(0); i <= blk.get(1); i += blk.getMultiplier())
            {
                assert(board[i] == true);
                board[i] = false;
            }
            blk.set(smallEdgeIndex);
            for(int i = blk.get(0); i <= blk.get(1); i += blk.getMultiplier())
            {
                assert(board[i] == false);
                board[i] = true;
            }
            hashes[blkIdx] = blk.hash();
            assert(hashes[blkIdx] != org_hash);
            for(int i = blocks.size() + 1; i < 18; ++i)
                assert(hashes[i] == 0);
        }

        bool canRedBlockUnblockInOneStep()
        {
            for(auto e : blocks)
            {
                if(e.getColor() == BlockColor::RED)
                {
                    bool rst = true;
                    for(int j = e.get(1) + 1; j < 18; ++j)
                    {
                        rst &= (board[j] == false);
                        if(rst == false)
                            return rst;
                    }
                }
            }
            return true;
        }

        void renderTheBoards(uint8_t *prettyBoard) const
        {
            for(int i = 0; i < 36; ++i)
            {
                if(board[i] == false)
                {
                    prettyBoard[i] = ' ';
                }
                else
                {
                    prettyBoard[i] = Board::cstring[getBlockIndex(i)];
                }
            }
        }
        
        Board backtrackTheBoard(Move& m)
        {
            Board boardBeforeOneMove(*this);
            uint8_t blkIdx = m.getBlockIndex();
            Block blk = boardBeforeOneMove.getBlock(static_cast<int>(blkIdx));
            auto moveDistance = m.getMoveDistance();

            // inverse the direction...
            // we are backtracking the move
            moveDistance = -moveDistance;
            auto originalEdge = blk.get(0) + moveDistance * blk.getMultiplier();
            boardBeforeOneMove.updateTheBoard(blkIdx, originalEdge);

            return boardBeforeOneMove;
        }

        /* ===== opertors ===== */
        // return the bit of input index
        bool operator[](uint32_t index) const
        {
            assert(index < 36);
            return board[index];
        }

        bool operator<(const Board& other) const
        {
            return memcmp(hashes, other.hashes, sizeof(hashes)) < 0;
        }

        friend ostream& operator<<(ostream& os, const Board& b);

        /* ===== static member function =====*/
        static void setAlgorithm(const char* c)
        {
            algorithm = c;
        }

        bool isGoal()
        {
            return board[16] == true && board[17] == true && 
                blocks[getBlockIndex(17)].getColor() == BlockColor::RED;
        }
    private:
        static const char* cstring;
        static const char* algorithm;
        uint32_t cost;
        unsigned hashes[18];
        bitset<36> board;
        vector<Block> blocks;
};
