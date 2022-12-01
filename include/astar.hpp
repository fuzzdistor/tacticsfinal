#ifndef TF_ASTAR_HPP
#define TF_ASTAR_HPP

#include "SFML/System/Vector2.hpp"
#include "map.hpp"
#include "utils.hpp"
#include <array>
#include <algorithm>
#include <optional>

struct Node
{
    explicit Node(const sf::Vector2u& coordinates);
    explicit Node(uint x, uint y);

    [[nodiscard]] constexpr uint getFCost() const;

    [[nodiscard]] static std::array<Node, 4> getNeighbours(const Node& n);

    sf::Vector2u coords{};
    uint g_cost{}; // cost of travel from START
    uint h_cost{}; // heuristic distance to FINISH
    Node* parent { nullptr };

    inline bool operator==(const Node& other) const;
};

constexpr uint Node::getFCost() const
{
    return g_cost + h_cost;
};

inline bool Node::operator==(const Node& other) const
{
    return coords == other.coords;
}

class AStar
{
public:
    explicit AStar(const Map& map);

    auto getPath(const sf::Vector2u& start,
                 const sf::Vector2u& goal,
                 uint movement,
                 Map::Terrain mask = Map::Terrain::Walkable)
                 -> std::optional<std::vector<sf::Vector2u>>;

private:
    Node& getLowestFNode();

    const Map& m_map;
    std::vector<Node> m_open{};
    std::vector<Node> m_closed{};
};

#endif // TF_ASTAR_HPP
