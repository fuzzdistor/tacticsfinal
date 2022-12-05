#ifndef TF_MAP_HPP
#define TF_MAP_HPP

#include "SFML/System/Vector2.hpp"
#include "astar.hpp"
#include "terrain.hpp"
#include <vector>
#include <iosfwd>
#include <string_view>
#include <cassert>

using uint = unsigned int;

struct MapData
{
    uint width;
    uint height;
    const char* map_string;
    const char* imagepath;
};

class Map
{
public:
    // Map constructor
    Map(const char map[], uint width, uint height);
    explicit Map(const MapData& map_data);

    Terrain::Type getTerrain(uint x, uint y) const;
    Terrain::Type getTerrain(const sf::Vector2u& coord) const;
    [[nodiscard]] sf::Vector2u getSize() const;

    void setTerrain(const sf::Vector2u& coord, Terrain::Type terrain);

    [[nodiscard]] constexpr bool isCoordInbounds(const sf::Vector2u& coord) const;

    [[nodiscard]] std::vector<sf::Vector2u> getRangeArea(const sf::Vector2u& center, uint radius, Terrain::Type mask = Terrain::Type::Walkable) const;
    [[nodiscard]] std::vector<sf::Vector2u> getRangeRadius(const sf::Vector2u& center, uint radius, Terrain::Type mask = Terrain::Type::Walkable) const;
    [[nodiscard]] AStar::Path getPath(const sf::Vector2u& start, const sf::Vector2u& goal, uint movement, Terrain::Type mask = Terrain::Type::Walkable) const;

private:
    friend constexpr size_t getCoordIndex(const Map& map, uint x, uint y);

#ifndef NDEBUG
    friend std::ostream& operator<<(std::ostream& os, const Map& map);
#endif // NDEBUG

    std::vector<Terrain::Type> m_terrain_layer;
    uint m_width;
    uint m_height;
    AStar m_pathfinder;
};

///////////////////////////////////////////////////////////////////////////////
// CONSTEXPR/INLINE DEFINITIONS
///////////////////////////////////////////////////////////////////////////////

constexpr size_t getCoordIndex(const Map& map, unsigned int x, unsigned int y)
{
    assert(x < map.m_width);
    assert(y < map.m_height);

    return x + y * map.m_width;
}

constexpr size_t getCoordIndex(const Map& map, const sf::Vector2u& coord)
{
    return getCoordIndex(map, coord.x, coord.y);
}

constexpr bool Map::isCoordInbounds(const sf::Vector2u& coord) const
{
    return coord.x < m_width && coord.y < m_height;
}

#endif // TF_MAP_HPP

