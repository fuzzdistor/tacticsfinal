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
#include "combatmanager.hpp"
#include "cursor.hpp"
#include "map.hpp"
#include "terrain.hpp"
#include "astar.hpp"
#include "resourcemanager.hpp"
#include "tilemarkers.hpp"
#include "turnmanager.hpp"
#include "team.hpp"
#include "tweener.hpp"
#include "unit.hpp"

using uint = unsigned int;

class Scene;

class Board : public sf::Drawable
{
public:
    Board() = delete;
    explicit Board(const MapData& data);

    template<typename T>
    void setEntityPosition(T& entity, const sf::Vector2u& movement, Terrain::Type mask = Terrain::Type::Walkable);

    void moveCharacter(Unit& unit, const sf::Vector2u& position);
    void setCursorPosition(const sf::Vector2u& position);
    void moveCursor(const sf::Vector2u& movement);
    void registerBattleResultCallback(std::function<void(bool)> callback);

    void accept();
    void cancel();

    void update(sf::Time time);

private:
    friend class AI;
    friend void imguiWidget(Board& board);
    friend void imguiWidget(Scene* scene);

    void reportDeath(uint id);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
    void updateTileMarkers();
    void advanceTurn(TurnManager::ActionTaken action);

    Map m_map;
    sf::Sprite m_sprite;
    TileMarkers m_tileMarkers;
    std::vector<Unit> m_units;
    TurnManager m_turnManager;
    const Unit* m_currentTurn;
    Cursor m_cursor;
    AI m_ai;
    Tweener& m_tweener;
    std::vector<std::pair<uint, uint>> m_aliveUnits;
    CombatManager m_combatManager;
    std::function<void(bool)> m_reportBattleResults;
    uint m_tileWidth {8};
    uint m_tileHeight {8};
    bool m_isOver = false;
    bool m_win = false;
};

#endif // TF_BOARD_HPP
