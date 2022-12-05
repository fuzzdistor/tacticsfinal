#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/System/Vector2.hpp"
#include "board.hpp"
#include "map.hpp"
#include "resourcemanager.hpp"
#include "terrain.hpp"
#include "turnmanager.hpp"
#include "utils.hpp"
#include <chrono>
#include <vector>

Board::Board(const MapData& data)
    : m_map(data)
    , m_playerTeam(6)
    , m_enemyTeam(6)
    , m_turnManager()
    , m_ai(*this)
    , m_tweener(Tweener::getInstance())
{
    m_playerTeam.emplaceUnit(ResourcePack::getInstance().textures.get(fd::hash("Allied")));
    auto& enemy = m_enemyTeam.emplaceUnit(ResourcePack::getInstance().textures.get(fd::hash("Enemy")));
    m_playerTeam[0].setName("Marche");
    m_playerTeam[0].setMovement(6);
    m_playerTeam[0].setSpeed(150);
    m_playerTeam[0].setPosition({11, 8});
    enemy.setName("Borzo");
    enemy.setMovement(6);
    enemy.setAwareness(10);
    enemy.setSpeed(130);
    enemy.setPosition({3, 3});

    auto& pack = ResourcePack::getInstance();

    m_cursor.sprite.setTexture(pack.textures.get(fd::hash("Cursor")));
    m_cursor.shape.setFillColor(sf::Color(0xAAAA00AA));

    pack.textures.load(fd::hash("Map1"), data.imagepath);
    m_sprite.setTexture(pack.textures.get(fd::hash("Map1")));

    for (const Unit& unit : m_playerTeam)
    {
        m_turnManager.registerUnit(&unit);
        m_map.setTerrain(unit.getPosition(), Terrain::Type::Unit);
    }

    for (const Unit& unit : m_enemyTeam)
    {
        m_turnManager.registerUnit(&unit);
        m_map.setTerrain(unit.getPosition(), Terrain::Type::Unit);
    }

    m_tileMarkers.updateHighlightedTiles(
            m_map.getRangeRadius(m_playerTeam.at(0).getPosition(), m_playerTeam.at(0).getMovement()));
    m_currentTurn = m_turnManager.getNextUnitAdvance();
}

template<typename T>
void Board::setEntityPosition(T& entity, const sf::Vector2u& position, Terrain::Type mask)
{
    /* DBG(entity.getPosition()); */
    /* DBG(position); */
    /* DBG(entity.getMovement()); */
    /* DBG(static_cast<uint>(mask)); */
    // recibo un Path. si el path es valido lo recorro, sino nada.
    if (auto path = m_map.getPath(entity.getPosition(), position, entity.getMovement(), mask); path.first == AStar::PathType::Valid)
    {
        // muevo la unidad a la posición de goal
        m_map.setTerrain(entity.getPosition(), Terrain::Type::Ground);
        entity.setPosition(path.second.front());
        m_map.setTerrain(entity.getPosition(), Terrain::Type::Unit);

        m_tileMarkers.updatePathMarkers(path.second);
        m_tileMarkers.updateHighlightedTiles(
                m_map.getRangeRadius(
                    m_currentTurn->getPosition()
                    , m_currentTurn->getMovement()
                    , Terrain::Type::Walkable));
    }
}

void Board::moveCharacter(Unit& unit, const sf::Vector2u& position)
{
    setEntityPosition(unit, position);
}

void Board::moveCursor(const sf::Vector2u& movement)
{
    m_cursor.position += movement;
}

void Board::setCursorPosition(const sf::Vector2u& position)
{
    m_cursor.position = position;
}


void Board::update(sf::Time dt)
{
    if (auto it = std::find_if(m_enemyTeam.begin(), m_enemyTeam.end(),
                [&](const Unit& unit) { return &unit == m_currentTurn; });
                it != m_enemyTeam.end())
    {
        D(TERM_CYAN << "ENEMY TAKES TURN!");
        m_ai.takeTurn(*it);
        m_currentTurn = m_turnManager.getNextUnitAdvance();
    }

    m_tweener.update(dt);
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
    m_tileMarkers.draw(target, states);
    m_playerTeam.at(0).draw(target, states);
    m_enemyTeam.at(0).draw(target, states);

    // TODO cursor
    states = sf::RenderStates();
    states.transform.translate(sf::Vector2f(m_cursor.position*8u));
    target.draw(m_cursor.shape, states);
    states.transform.translate(sf::Vector2f(0, -7.f));
    target.draw(m_cursor.sprite, states);
}

void Board::accept()
{
    if (auto it = std::find_if(m_playerTeam.begin(), m_playerTeam.end(),
                [&](const Unit& unit) { return &unit == m_currentTurn; });
            it != m_playerTeam.end())
    {
        if (auto path = m_map.getPath(m_playerTeam.at(0).getPosition(), m_cursor.position, m_playerTeam.at(0).getMovement());
                path.first == AStar::PathType::Valid)
        {
            D(TERM_ORANGE << "PLAYER TAKES TURN!");
            moveCharacter(m_playerTeam.at(0), m_cursor.position);
            m_turnManager.takeCtFromUnit(&m_playerTeam.at(0), TurnManager::ActionTaken::Moved);
            m_currentTurn = m_turnManager.getNextUnitAdvance();
        }
    }
}

void Board::cancel()
{
}
