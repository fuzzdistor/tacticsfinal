#ifndef TF_ASTAR_HPP
#define TF_ASTAR_HPP

#include "SFML/System/Vector2.hpp"
#include "utils.hpp"
#include "terrain.hpp"
#include <array>
#include <algorithm>
#include <optional>
#include <variant>

class Map;

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
    enum class PathType
    {
        Valid,
        OutOfMap,
        Unreachable,
    };

    typedef std::pair<PathType, std::vector<sf::Vector2u>> Path;

    explicit AStar(const Map& map);

    Path getPath(const sf::Vector2u& start,
                 const sf::Vector2u& goal,
                 uint movement,
                 Terrain::Type mask = Terrain::Type::Walkable) const;

private:
    Node& getLowestFNode() const;

    const Map& m_map;
    mutable std::vector<Node> m_open{};
    mutable std::vector<Node> m_closed{};
};

#endif // TF_ASTAR_HPP
