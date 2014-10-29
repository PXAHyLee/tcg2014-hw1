#pragma once

#include "common.h"

#include <bitset>
#include <cassert>
#include <cstdint>

using std::bitset;

enum class BlockDirection { HORIZONTAL, VERTICAL, UNDEFINED};
enum class BlockColor { RED, BROWN, UNDEFINED};

class Block
{
    public:
        Block() : color(BlockColor::UNDEFINED),
            direction(BlockDirection::UNDEFINED), length(0),
            edge(0), multiplier(0), globalId(0) { }

        Block(BlockColor c, BlockDirection dir, uint8_t len, uint8_t e, int gid);
        Block(BlockColor c, BlockDirection dir, uint8_t len, uint8_t e);

        friend ostream& operator<<(ostream& os, const Block& b);

        uint8_t size() const { return length; }

        uint8_t get(int index) const
        {
            assert(index == 0 || index == 1);
            return index == 0 ? edge : edge + (length - 1) * multiplier;
        }

        void set(int index)
        {
            edge = index;
        }

        const uint8_t getMultiplier() const
        {
            return multiplier;
        }

        // moves distances of this block
        // fill two directions in the array
        void fillIndices(bitset<36>& bitmap);
        BlockDirection getDirection() const { return direction; }
        bool isHorizontal() const { return direction == BlockDirection::HORIZONTAL; } 
        bool isVertical() const { return direction == BlockDirection::VERTICAL; }
        bool isBelongsToThisBlock(int boardIndex) const
        {
            if(length == 3) {
                return get(0) == boardIndex ||
                    get(0) + multiplier == boardIndex ||
                    get(1) == boardIndex;
            }
            return get(0) == boardIndex || get(1) == boardIndex;
        }
        void moveDistance(const bitset<36>& bitmap, int* twoDirection) const;
        const BlockColor getColor() const
        {
            return color;
        }
        unsigned hash()
        {
            return (globalId << 8 | edge);
        }
        int unit_test();
    private:
        // helper functions of moveDistance
        bool isBoundary(int index) const
        {
            if(direction == BlockDirection::HORIZONTAL)
            {
                int rst = index % 6;
                return rst == 0 || rst == 5;
            }
            return index < 6 || index >= 30;
        }
        int moveLeft(const bitset<36>& bitmap, int start_point) const;
        int moveRight(const bitset<36>& bitmap, int start_point) const;
        int moveUp(const bitset<36>& bitmap, int start_point) const;
        int moveDown(const bitset<36>& bitmap, int start_point) const;

    private:
        BlockColor color;
        BlockDirection direction; // horizontal or vertical
        uint8_t length; // the block size: 2 or 3
        uint8_t edge; // the top-left of this block
        uint8_t multiplier;
        uint8_t globalId;
};
