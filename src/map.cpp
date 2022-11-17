#include "map.hpp"
#include "utils.hpp"
#include "SFML/System/Vector2.hpp"
#include <cassert>
#include <iostream>
#include <vector>

Map::Terrain charToTerrain(char tile_char)
{
    switch(tile_char)
    {
        case 'w': return Map::Terrain::Water;
        case 'x': return Map::Terrain::Obstacle;
        case '.': return Map::Terrain::Ground;
        case '-': return Map::Terrain::None;
    }
    return Map::Terrain::None;
}

size_t getMapIndex(const Map& map, unsigned int x, unsigned int y)
{
    assert(x < map.m_width);
    assert(y < map.m_height);

    return x + y * map.m_width;
}

Map::Map(char map[], uint width, uint height)
    : m_map()
    , m_width(width)
    , m_height(height)
{
    m_map.reserve(width * height);
    for (uint i = 0; i < width*height; i++)
    {
        m_map.push_back(charToTerrain(map[i]));
    }
}

const std::vector<sf::Vector2u> Map::getRangeRadius(const sf::Vector2u center, unsigned int radius, Terrain mask) const
{
    assert(center.x < m_width);
    assert(center.y < m_height);

    if (radius == 0)
    {
        if (getTerrain(center) == Map::Terrain::None)
            return {};
        else
            return { sf::Vector2u(center.x, center.y) };
    }

    DBG(center.x);
    DBG(center.y);

    std::vector<sf::Vector2u> range;

    for (uint y = center.y - radius; y <= std::min(center.y + radius, m_height - 1) || y >= -radius; y++)
    {
        // si estoy haciendo underflow o me pase del alto del mapa no
        // me importa esta fila. continuo a la siguiente
        if (y >= m_height)
            continue;

        for (uint x = center.x - radius; x <= center.x + radius || x >= -radius; x++)
        {
            // si estoy haciendo underflow o me pase del ancho del mapa no
            // me importa esta columna. continuo a la siguiente
            if (x >= m_width)
                continue;

            if (masksMatch(getTerrain(x,y), mask))
                range.emplace_back(x,y);
        }
    }

    return range;
}

Map::Terrain Map::getTerrain(unsigned int x, unsigned int y) const
{
    return m_map.at(getMapIndex(*this, x, y));
}

Map::Terrain Map::getTerrain(const sf::Vector2u& coord) const
{
    return m_map.at(getMapIndex(*this, coord.x, coord.y));
}

#ifndef NDEBUG
std::ostream& operator<<(std::ostream& os, const Map& map)
{
    os << "Map: {\n  std::vector<Terrain> m_map: {\n    size: " << map.m_map.size()
    << "\n  },\n  uint m_width: " << map.m_width << ",\n  uint m_height: " << map.m_height
    << "\n}";
    return os;
}
#endif // NDEBUG

