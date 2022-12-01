#ifndef TF_BOARD_HPP
#define TF_BOARD_HPP

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Vector2.hpp"
#include "map.hpp"
#include "astar.hpp"
#include "resourcemanager.hpp"
#include "team.hpp"
#include "unit.hpp"

using uint = unsigned int;

struct Cursor
{
    sf::Vector2u position{};
    sf::Sprite sprite{};
    sf::RectangleShape shape{{8, 8}};
};

class Board : public sf::Drawable
{
public:
    explicit Board(const MapData& data);

    template<typename T>
    void setEntityPosition(T& entity, const sf::Vector2u& movement, Map::Terrain mask = Map::Terrain::Walkable);

    void moveCharacter(const sf::Vector2u& movement);
    void moveCursor(const sf::Vector2u& movement);
    void setCursorPosition(const sf::Vector2u& position);

    void accept();
    void cancel();

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
    void updateHighlightedTiles();
    void updatePathRects(const std::vector<sf::Vector2u>& path);

    void turnTick();

    Map m_map;
    sf::Sprite m_sprite {};
    std::vector<sf::RectangleShape> m_tile_rects {};
    std::vector<sf::RectangleShape> m_path_rects {};
    uint m_tile_width {8};
    uint m_tile_height {8};
    Team m_playerTeam;
    Team m_enemyTeam;
    Cursor m_cursor {{3, 3}};
    AStar m_pathfinder;
};

#endif // TF_BOARD_HPP
