#include "block.hpp"

using std::vector;

Block::Block(BlockColor c, BlockDirection dir, uint8_t len,
             uint8_t e, int gid) :
             color(c),
             direction(dir),
             length(len),
             edge(e),
             multiplier((direction == BlockDirection::VERTICAL) ? 6 : 1),
             globalId(gid) { }

Block::Block(BlockColor c, BlockDirection dir, uint8_t len, uint8_t e) :
    color(c), direction(dir), length(len), edge(e), 
    multiplier((direction == BlockDirection::VERTICAL) ? 6 : 1) { }

ostream& operator<<(ostream& os, const Block& b)
{
    if(b.color == BlockColor::RED)
        os << "RED ";
    else
        os << "BROWN ";
    if(b.length == 3) {
        assert(b.get(1) - b.get(0) == 2 * b.multiplier);
        return os << static_cast<int>(b.get(0)) << " "
            << static_cast<int>(b.get(0) + b.multiplier) << " "
            << static_cast<int>(b.get(1)) << endl;
    }
    return os << static_cast<int>(b.get(0)) << " "
        << static_cast<int>(b.get(1)) << endl;
}

void Block::fillIndices(bitset<36>& bitmap)
{
    assert((bitmap[get(0)] || bitmap[get(1)]) == 0);
    bitmap[get(0)] = bitmap[get(1)] = 1;
    if(length > 2) {
        assert(bitmap[get(0) + multiplier] == 0);
        bitmap[get(0) + multiplier] = 1;
    }
}

// return the leftmost index that this block can move
int Block::moveLeft(const bitset<36>& bitmap, int start_point) const
{
    // this block cannot move left
    if(isBoundary(start_point) == true ||
            (start_point > 0 && bitmap[start_point - 1] == true))
        return start_point;

    // this block can at least move left by one unit
    int i = start_point - 1;
    assert(i >= 0);
    while(bitmap[i] == false)
    {
        if(isBoundary(i) == true)
            return i;
        i--;
    }
    return i + 1;
}

int Block::moveRight(const bitset<36>& bitmap, int start_point) const
{
    // this block cannot move right
    if(isBoundary(start_point) == true ||
            bitmap[start_point + 1] == true)
        return start_point;

    // this block can at least move right by one unit
    int i = start_point + 1;
    assert(i >= 0);
    while(bitmap[i] == false)
    {
        if(isBoundary(i) == true)
            return i;
        i++;
    }
    return i - 1;
}

int Block::moveUp(const bitset<36>& bitmap, int start_point) const
{
    // this block cannot move up
    if(isBoundary(start_point) == true || bitmap[start_point - 6] == true)
        return start_point;

    // this block can at least move up by one unit
    int i = start_point - 6;
    assert(i >= 0);
    while(bitmap[i] == false)
    {
        if(isBoundary(i) == true)
            return i;
        i -= 6;
    }
    return i + 6;
}

int Block::moveDown(const bitset<36>& bitmap, int start_point) const
{
    // this block cannot move down
    if(isBoundary(start_point) == true || bitmap[start_point + 6] == true)
        return start_point;

    // this block can at least move down by one unit
    int i = start_point + 6;
    assert(i >= 0);
    while(bitmap[i] == false)
    {
        if(isBoundary(i) == true)
            return i;
        i += 6;
    }
    return i - 6;
}

void Block::moveDistance(const bitset<36>& bitmap, int* twoDirection) const
{
    if(direction == BlockDirection::HORIZONTAL)
    {
        twoDirection[0] = moveLeft(bitmap, get(0));
        twoDirection[1] = moveRight(bitmap, get(1));
    }
    else
    {
        twoDirection[0] = moveUp(bitmap, get(0));
        twoDirection[1] = moveDown(bitmap, get(1));
    }
    // cout << __PRETTY_FUNCTION__ << ": " << twoDirection[0] << " " <<
    //     twoDirection[1] << endl;
}

int Block::unit_test() {
    cout << "Block unit test" << endl;
    cout << "Block size: " << static_cast<int>(size()) << endl;
    cout << "First: " << static_cast<int>(get(0)) << endl;
    cout << "Second: " << static_cast<int>(get(1)) << endl;
    cout << "is horizontal? " << static_cast<int>(isHorizontal()) << endl;
    cout << "is Vertical? " << static_cast<int>(isVertical()) << endl;
    cout << "globalId " << static_cast<int>(globalId) << endl;
    cout << "hash of the block: " << hash() << endl;
    return 0;
}
