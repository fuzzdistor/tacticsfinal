#ifndef TF_AI_HPP
#define TF_AI_HPP

#include "astar.hpp"
#include "xorshift.hpp"
#include <algorithm>
#include <vector>

class Unit;
class Board;

class AI
{
public:
    AI(Board& board);
    void takeTurn(Unit& unit);

private:
    Board& m_board;
    xorshift m_rng;
};

#endif // TF_AI_HPP
