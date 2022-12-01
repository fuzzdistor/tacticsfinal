#ifndef TF_MAP_HPP
#define TF_MAP_HPP

#include "SFML/System/Vector2.hpp"
#include <vector>
#include <iosfwd>
#include <string_view>

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
    enum class Terrain : uint
    {
        Ground = 1 << 0,
        Water = 1 << 1,
        None = 1 << 2,
        Obstacle = 1 << 3,
        Unit = 1 << 4,

        Walkable = Ground | Water | None,
        Castable = Ground | Water | Unit | None,
        All = 0xFFFFFFFF,
    };

    Map(const char map[], uint width, uint height);
    explicit Map(const MapData& map_data);

    [[nodiscard]] Terrain getTerrain(uint x, uint y) const;
    [[nodiscard]] Terrain getTerrain(const sf::Vector2u& coord) const;
    [[nodiscard]] sf::Vector2u getSize() const;

    void setTerrain(const sf::Vector2u& coord, Terrain terrain);
    void setObject(const sf::Vector2u& coord, Terrain terrain);

    [[nodiscard]] constexpr bool isCoordInbounds(const sf::Vector2u& coord) const
    {
        return coord.x < m_width && coord.y < m_height;
    }

    [[nodiscard]] std::vector<sf::Vector2u> getRangeArea(const sf::Vector2u& center, uint radius, Terrain mask = Terrain::Walkable) const;
    [[nodiscard]] std::vector<sf::Vector2u> getRangeRadius(const sf::Vector2u& center, uint radius, Terrain mask = Terrain::Walkable) const;

private:
    friend constexpr size_t getCoordIndex(const Map& map, uint x, uint y);
#ifndef NDEBUG
    friend std::ostream& operator<<(std::ostream& os, const Map& map);
#endif // NDEBUG

    std::vector<Terrain> m_terrain_layer;
    std::vector<Terrain> m_objects_layer;
    uint m_width;
    uint m_height;
};

#endif // TF_MAP_HPP

