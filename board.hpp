#pragma once

#include "common.h"
#include "move.hpp"
#include "block.hpp"

#include <cstring>
#include <string>
#include <cstdint>
#include <cassert>
#include <bitset>
#include <list>
#include <utility>

using std::bitset;
using std::list;
using std::pair;

class SearchAlgorithm;
class Board;

typedef pair<vector<Board>, vector<Move>> BoardsAndMoves;

class Board
{
    public:
        static const char* algorithm;

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

        // don't let compiler generate copy ctor
        Board(const Board& other) : cost(other.cost), board(other.board)
        {
            memcpy(hashes, other.hashes, sizeof(hashes));
            blocks.reserve(static_cast<int>(other.blocks.size()));
            for(uint8_t i = 0; i < other.blocks.size(); ++i)
            {
                blocks.push_back(other.blocks[i]);
                hashes[i] = other.blocks[i].hash();
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

        void updateTheBoard(int blkIdx, int smallEdgeIndex);

        void renderTheBoards(uint8_t *prettyBoard) const;
        
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

        // std::set and std::map
        bool operator<(const Board& other) const
        {
            return memcmp(hashes, other.hashes, sizeof(hashes)) < 0;
        }

        // priority_queue (std::greater)
        // a > b: Board a may take more cost to reach the solution than Board b
        bool operator>(const Board& other) const
        {
            uint32_t val = cost + numberOfObstacle();
            uint32_t other_val = other.cost + other.numberOfObstacle();
            return val > other_val;
        }

        /* ===== list of heuristic functions =====*/
        int numberOfObstacle() const;

        friend ostream& operator<<(ostream& os, const Board& b);

        /* ===== static member function =====*/
        static void setAlgorithm(const char* c) { algorithm = c; }

        bool isGoal()
        {
            return board[16] == true && board[17] == true && 
                blocks[getBlockIndex(17)].getColor() == BlockColor::RED;
        }
    private:
        static const char* cstring;
        uint32_t cost;
        unsigned hashes[18];
        bitset<36> board;
        vector<Block> blocks;
};
