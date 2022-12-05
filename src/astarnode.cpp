#include "astarnode.hpp"

AStarNode::AStarNode(const sf::Vector2u& coordinates)
    : coords(coordinates)
{
}

AStarNode::AStarNode(uint x, uint y)
    : coords(x, y)
{
}

auto AStarNode::getNeighbours(const AStarNode& n) -> std::array<AStarNode, 4>
{
    return {
        AStarNode(n.coords.x    , n.coords.y - 1), // top
        AStarNode(n.coords.x - 1, n.coords.y    ), // left
        AStarNode(n.coords.x + 1, n.coords.y    ), // right
        AStarNode(n.coords.x    , n.coords.y + 1), // bottom
    };
}

