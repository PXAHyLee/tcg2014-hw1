#include <fstream>
#include <iostream>
#include <vector>
#include <bitset>
#include <cstdint>
#include <cstdlib>
#include "parser.hpp"
using std::vector;
using std::bitset;

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

static inline BlockDirection currentDirection(const char& c)
{
    if(c == 'v')
        return BlockDirection::VERTICAL;
    else if(c == 'h')
        return BlockDirection::HORIZONTAL;
    else
    {
        cout << __PRETTY_FUNCTION__ << ": something goes wrong..." << endl;
        exit(1);
    }
}

static inline BlockColor currentColor(const char& c)
{
    if(likely(c == 'b'))
        return BlockColor::BROWN;
    else if(c == 'r')
        return BlockColor::RED;
    else
    {
        cout << __PRETTY_FUNCTION__ << ": something goes wrong..." << endl;
        exit(1);
    }
}

Board Parser::getBoardFromFile()
{
    int numberOfBlocks {0}, i {0};
    Block blks[15];
    bitset<36> b;

    input_file >> numberOfBlocks;
    while(i < numberOfBlocks)
    {
        int edge {0}, length {0};
        char d, c;

        input_file >> edge >> length >> d >> c;

        blks[i] = Block(currentColor(c), currentDirection(d), length, edge, i);
        blks[i].fillIndices(b);
        i++;
    }
    return Board(b, numberOfBlocks, blks);
}
