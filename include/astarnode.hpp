#ifndef TF_ASTARNODE_HPP
#define TF_ASTARNODE_HPP

#include "SFML/System/Vector2.hpp"
#include <array>

using uint = unsigned int;

struct AStarNode
{
    explicit AStarNode(const sf::Vector2u& coordinates);
    explicit AStarNode(uint x, uint y);

    [[nodiscard]] constexpr uint getFCost() const;

    [[nodiscard]] static std::array<AStarNode, 4> getNeighbours(const AStarNode& n);

    sf::Vector2u coords{};
    uint g_cost{}; // cost of travel from START
    uint h_cost{}; // heuristic distance to FINISH
    AStarNode* parent { nullptr };

    inline bool operator==(const AStarNode& other) const;
};

constexpr uint AStarNode::getFCost() const
{
    return g_cost + h_cost;
};

inline bool AStarNode::operator==(const AStarNode& other) const
{
    return coords == other.coords;
}

#endif // TF_ASTARNODE_HPP
