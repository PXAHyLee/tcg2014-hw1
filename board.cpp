#include "board.hpp"

const char* Board::algorithm = 0;
const char* Board::cstring = "ABCDEFGHIJKLMNO";

BoardsAndMoves Board::moves()
{
    vector<Board> boards;
    vector<Move> moves;
    uint8_t prettyBoard[36] {0};

    renderTheBoards(prettyBoard);
    for(int i = 0; i < blocks.size(); i++)
    {
        int twoDirection[2] {-1, -1};

        blocks[i].moveDistance(board, twoDirection);
        assert(twoDirection[0] >= 0 && twoDirection[1] >= 0);
        for(int j = twoDirection[0]; j != blocks[i].get(0); j += blocks[i].getMultiplier())
        {
            boards.emplace_back(Board(*this));
            boards.back().updateTheBoard(i, j);
            boards.back().increaseCost();
            moves.emplace_back(Move(static_cast<uint8_t>(i),
                    (j - blocks[i].get(0)) / blocks[i].getMultiplier(),
                    blocks[i].getDirection(),
                    prettyBoard));
        }

        for(int j = twoDirection[1]; j != blocks[i].get(1); j -= blocks[i].getMultiplier())
        {
            boards.emplace_back(Board(*this));
            boards.back().updateTheBoard(i,
                    j - (blocks[i].size() - 1) * blocks[i].getMultiplier());
            boards.back().increaseCost();
            moves.emplace_back(Move(static_cast<uint8_t>(i),
                    (j - blocks[i].get(1)) / blocks[i].getMultiplier(),
                    blocks[i].getDirection(),
                    prettyBoard));
        }
    }
    return make_pair(boards, moves);
}

int Board::getBlockIndex(int boardIndex) const
{
    if(board[boardIndex] == false)
        return -1;
    for(int i = 0; i < blocks.size(); ++i)
    {
        if(blocks[i].isBelongsToThisBlock(boardIndex))
            return i;
    }
    return -1;
}

void Board::updateTheBoard(int blkIdx, int smallEdgeIndex)
{
    Block& blk = blocks[blkIdx];
    unsigned org_hash = blk.hash();
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

void Board::renderTheBoards(uint8_t *prettyBoard) const
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

// a friend function isn't a member function of Board!
ostream& operator<<(ostream& os, const Board& b)
{
    auto tmpBoard = b.getBoard();
    auto cstring = "ABCDEFGHIJKLMN";
    for(int i = 0; i < 6; i++) {
        for(int j = 0; j < 6; j++)
        {
            if(tmpBoard[j] == false)
                os << ' ';
            else
                os << cstring[b.getBlockIndex(i*6 + j)];
        }
        tmpBoard >>= 6;
        os << endl;
    }
    return os;
}


int Board::numberOfObstacle() const
{
    int obstacle {0};
    for(auto e : blocks)
    {
        if(e.getColor() == BlockColor::RED)
        {
            // don't count the brown block behind the red block
            for(int i = e.get(0); i < 18; ++i)
            {
                auto blkIdx = getBlockIndex(i);
                if(blkIdx >= 0 &&
                        getBlock(blkIdx).getColor() == BlockColor::BROWN)
                    obstacle++;
            }
        }
    }

    return obstacle;
}
