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

Map::Map(const char map[], uint width, uint height)
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

const std::vector<sf::Vector2u> Map::getRangeArea(const sf::Vector2u center, unsigned int range, Terrain mask) const
{
    // center tiene que estar dentro del mapa como prerequisito. usar un center fuera del mapa resulta en undefined behaviour
    assert(center.x < m_width);
    assert(center.y < m_height);

    if (range == 0)
    {
        if (masksMatch(getTerrain(center), mask))
            return { center };
        else
            return {};
    }

    std::vector<sf::Vector2u> range_v;

    for (uint y = center.y - range; y <= std::min(center.y + range, m_height - 1) || y >= -range; y++)
    {
        // si estoy haciendo underflow o me pase del alto del mapa no
        // me importa esta fila. continuo a la siguiente
        if (y >= m_height)
            continue;

        for (uint x = center.x - range; x <= center.x + range || x >= -range; x++)
        {
            // si estoy haciendo underflow o me pase del ancho del mapa no
            // me importa esta columna. continuo a la siguiente
            if (x >= m_width)
                continue;

            if (masksMatch(getTerrain(x,y), mask))
                range_v.emplace_back(x,y);
        }
    }

    return range_v;
}

const std::vector<sf::Vector2u> Map::getRangeRadius(const sf::Vector2u center, unsigned int range, Terrain mask) const
{
    // center tiene que estar dentro del mapa como prerequisito. usar un center fuera del mapa resulta en undefined behaviour
    assert(center.x < m_width);
    assert(center.y < m_height);

    if (range == 0)
    {
        if (masksMatch(getTerrain(center), mask))
            return { center };
        else
            return {};
    }

    std::vector<sf::Vector2u> range_v;

    for (uint y = center.y - range; y <= std::min(center.y + range, m_height - 1) || y >= -range; y++)
    {
        // si estoy haciendo underflow o me pase del alto del mapa no
        // me importa esta fila. continuo a la siguiente
        if (y >= m_height)
            continue;

        uint skip_tiles = y > center.y ? y - center.y : center.y - y;
        for (uint x = center.x - range + skip_tiles; x <= center.x + range - skip_tiles || x >= -range; x++)
        {
            // si estoy haciendo underflow o me pase del ancho del mapa no
            // me importa esta columna. continuo a la siguiente
            if (x >= m_width)
                continue;

            if (masksMatch(getTerrain(x,y), mask))
                range_v.emplace_back(x,y);
        }
    }

    return range_v;
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

