#ifndef TF_MAP_HPP
#define TF_MAP_HPP

#include "SFML/System/Vector2.hpp"
#include <vector>
#include <iosfwd>

#define uint unsigned int

class Map
{
public:
    // Map constructor
    enum class Terrain
    {
        Ground = 1 << 0,
        Water = 1 << 1,
        None = 1 << 2,
        Obstacle = 1 << 3,

        Walkable = Ground | Water,
        Castable = Ground | Water | Obstacle,
    };

    Map(const char map[], uint width, uint height);

    Terrain getTerrain(uint x, uint y) const;
    Terrain getTerrain(const sf::Vector2u& coord) const;

    const std::vector<sf::Vector2u> getRangeArea(const sf::Vector2u center, uint radius, Terrain mask = Terrain::Walkable) const;
    const std::vector<sf::Vector2u> getRangeRadius(const sf::Vector2u center, uint radius, Terrain mask = Terrain::Walkable) const;

private:
    friend size_t getMapIndex(const Map& map, uint x, uint y);
#ifndef NDEBUG
    friend std::ostream& operator<<(std::ostream& os, const Map& map);
#endif // NDEBUG

    std::vector<Terrain> m_map;
    uint m_width;
    uint m_height;
};

#endif // TF_MAP_HPP
