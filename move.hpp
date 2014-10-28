#pragma once

#include "block.hpp"
#include <cassert>
#include <iostream>
#include <cstring>
#include <bitset>

using std::cout;
using std::endl;

class Move
{
    public:
        Move(uint8_t idx = 0, int8_t len = 0,
                BlockDirection dir = BlockDirection::UNDEFINED,
                const uint8_t* pBoard = NULL) :
            blockIdx(idx), moveDistance(len), direction(dir)
        {
            if(pBoard != NULL)
            {
                memcpy(prettyBoard, pBoard, sizeof(prettyBoard));
            }
        }

        Move(const Move& other) : blockIdx(other.blockIdx),
            moveDistance(other.moveDistance), direction(other.direction)
        {
            memcpy(prettyBoard, other.prettyBoard, sizeof(prettyBoard));
        }

        uint8_t getBlockIndex() const { return blockIdx; }
        int8_t getMoveDistance() const { return moveDistance; }
        BlockDirection getDirection() const { return direction; }

        /* ===== operators =====*/
        friend ostream& operator<<(ostream& os, const Move& move)
        {
            auto d = static_cast<int>(move.moveDistance);
            assert(move.moveDistance != 0);

            os << "===a move: move block " << Move::cstring[move.blockIdx];
            if(move.direction == BlockDirection::HORIZONTAL)
            {
                if(move.moveDistance < 0)
                    os << " left for " << -d << " unit.===";
                else
                    os << " right for " << d << " unit.===";
            }
            else
            {
                if(move.moveDistance < 0)
                    os << " up for " << -d << " unit.===";
                else
                    os << " down for " << d << " unit.===";
            }
            cout << endl;
            return os;
        }
    private:
        static const char* cstring;
        uint8_t blockIdx;
        // distance can be smaller than zero if moving the block left(up)
        uint8_t prettyBoard[36];
        int8_t moveDistance;
        BlockDirection direction;
};
