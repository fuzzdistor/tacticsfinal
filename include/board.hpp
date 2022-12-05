#ifndef TF_BOARD_HPP
#define TF_BOARD_HPP
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Time.hpp"
#include "SFML/System/Vector2.hpp"
#include "ai.hpp"
#include "map.hpp"
#include "terrain.hpp"
#include "astar.hpp"
#include "resourcemanager.hpp"
#include "turnmanager.hpp"
#include "team.hpp"
#include "tweener.hpp"
#include "unit.hpp"

using uint = unsigned int;

class Scene;

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
    void setEntityPosition(T& entity, const sf::Vector2u& movement, Terrain::Type mask = Terrain::Type::Walkable);

    void moveCharacter(Unit& unit, const sf::Vector2u& position);
    void moveCursor(const sf::Vector2u& movement);
    void setCursorPosition(const sf::Vector2u& position);

    void accept();
    void cancel();

    void update(sf::Time time);

private:
    friend class AI;
    friend void imguiWidget(Scene* scene);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
    void updateHighlightedTiles();
    void updatePathRects(const std::vector<sf::Vector2u>& path);

    Map m_map;
    sf::Sprite m_sprite {};
    std::vector<sf::RectangleShape> m_tileRects {};
    std::vector<sf::RectangleShape> m_pathRects {};
    uint m_tileWidth {8};
    uint m_tileHeight {8};
    Team m_playerTeam;
    Team m_enemyTeam;
    TurnManager m_turnManager;
    Cursor m_cursor {{3, 3}};
    const Unit* m_currentTurn;
    AI m_ai;
    Tweener m_tweener;
};

#endif // TF_BOARD_HPP
