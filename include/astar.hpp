#ifndef TF_ASTAR_HPP
#define TF_ASTAR_HPP

#include "SFML/System/Vector2.hpp"
#include "utils.hpp"
#include "terrain.hpp"
#include "astarnode.hpp"
#include <array>
#include <algorithm>
#include <optional>
#include <variant>

class Map;

class AStar
{
public:
    enum class PathType
    {
        Valid,
        Invalid,
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
    AStarNode& getLowestFNode() const;

    const Map& m_map;
    mutable std::vector<AStarNode> m_open{};
    mutable std::vector<AStarNode> m_closed{};
};

#endif // TF_ASTAR_HPP
