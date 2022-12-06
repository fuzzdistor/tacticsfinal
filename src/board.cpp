#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/System/Vector2.hpp"
#include "board.hpp"
#include "map.hpp"
#include "resourcemanager.hpp"
#include "terrain.hpp"
#include "units.data.hpp"
#include "turnmanager.hpp"
#include "utils.hpp"
#include <chrono>
#include <imgui.h>
#include <vector>

Board::Board(const MapData& data)
    : m_map(data)
    , m_tileMarkers()
    , m_units()
    , m_turnManager()
    , m_ai(*this)
    , m_tweener(Tweener::getInstance())
{
    m_units.reserve(data.units);
    {
        std::array<sf::Vector2u, data_units> coordinates =
        {
            sf::Vector2u(11, 8),
            sf::Vector2u(10, 8),
            sf::Vector2u(9, 8),
            sf::Vector2u(8, 8),
            sf::Vector2u(3, 0),
            sf::Vector2u(2, 0),
            sf::Vector2u(1, 0),
            sf::Vector2u(0, 0),
        };
        for (size_t i = 0; i < data_units; i++)
        {
            auto hash = data_playercontrolled[i]? fd::hash("Allied") : fd::hash("Enemy");
            auto faction = data_playercontrolled[i]? 1u : 2u;
            auto& unit = m_units.emplace_back(
                    ResourcePack::getInstance().textures.get(hash)
                    , data_names[i]
                    , data_stats[i]
                    );
            unit.setCoordinates(coordinates[i]);
            unit.setPlayerControlled(data_playercontrolled[i]);
            unit.setFaction(faction);
        }
    }

    auto& pack = ResourcePack::getInstance();

    pack.textures.load(fd::hash("Map1"), data.imagepath);
    m_sprite.setTexture(pack.textures.get(fd::hash("Map1")));

    for (const Unit& unit : m_units)
    {
        m_turnManager.registerUnit(&unit);
        m_map.setTerrain(unit.getCoordinates(), Terrain::Type::Unit);
    }

    m_currentTurn = m_turnManager.getNextUnitAdvance();
    m_tileMarkers.updateHighlightedTiles(m_map.getRangeRadius(m_currentTurn->getCoordinates(), m_currentTurn->getStats().movement));
    m_cursor.setCoordinates(m_currentTurn->getCoordinates());
}

template<typename T>
void Board::setEntityPosition(T& entity, const sf::Vector2u& position, Terrain::Type mask)
{
    // recibo un Path. si el path es valido lo recorro, sino nada.
    D("attempting to move");
    if (auto path = m_map.getPath(entity.getCoordinates(), position, entity.getStats().movement, mask); path.first == AStar::PathType::Valid)
    {
        D("attempt successful");
        // muevo la unidad a la posición de goal
        m_map.setTerrain(entity.getCoordinates(), Terrain::Type::Ground);
        m_map.setTerrain(path.second.front(), Terrain::Type::Unit);

        entity.tweenPath(path.second);
        //entity.tweenPosition(path.second.front());
        m_tileMarkers.updatePathMarkers(path.second);
    }
    else
        D("attempt failed for some reason");
}

void Board::advanceTurn()
{
    auto it = std::find_if(
            m_units.begin()
            , m_units.end()
            , [&](const Unit& unit) { return &unit == m_currentTurn; });
    m_turnManager.takeCtFromUnit(*it, TurnManager::ActionTaken::Moved);
    m_currentTurn = m_turnManager.getNextUnitAdvance();
    m_map.updateTerrainFaction(m_units, m_currentTurn->getFaction());
    updateTileMarkers();
    m_cursor.tweenPosition(m_currentTurn->getCoordinates());
    m_tweener.createTween([t = sf::Time::Zero](sf::Time dt) mutable
            { t+=dt; return t > sf::seconds(1.f); });
}

void Board::moveCharacter(Unit& unit, const sf::Vector2u& position)
{
    setEntityPosition(unit, position);
}

void Board::updateTileMarkers()
{
    m_tileMarkers.updateTargetTile(m_currentTurn->getCoordinates());
    m_tileMarkers.updateHighlightedTiles(
            m_map.getRangeRadius(
                m_currentTurn->getCoordinates()
                , m_currentTurn->getStats().movement
                , Terrain::Type::Walkable));
}

void Board::update(sf::Time)
{
    if (!m_tweener.isActive() && !m_currentTurn->isPlayerControlled())
    {
        auto it = std::find_if(
                m_units.begin()
                , m_units.end()
                , [&](const Unit& unit) { return &unit == m_currentTurn; });

        m_ai.takeTurn(*it);
    }

    Stats s = m_currentTurn->getStats();
    ImGui::Begin("Current Turn");
    ImGui::Text("Unit: %s", m_currentTurn->getName().c_str());
    ImGui::Text("HP: %d/%d", s.healthPoints, s.maxHealthPoints);
    ImGui::Text("MP: %d/%d", s.magicPoints, s.maxMagicPoints);
    ImGui::Text("Lvl/Exp: %d/%d", s.level, s.experiencePoints);
    ImGui::End();
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
    m_tileMarkers.draw(target, states);
    for (auto& unit : m_units)
        unit.draw(target, states);

    m_cursor.draw(target, states);
}

void Board::setCursorPosition(const sf::Vector2u& position)
{
    m_cursor.setCoordinates(position);
}

void Board::moveCursor(const sf::Vector2u& movement)
{
    m_cursor.tweenPosition(m_cursor.getCoordinates() + movement);
}

void Board::accept()
{
    if (!m_tweener.isActive() && m_currentTurn->isPlayerControlled())
    {
        if (auto path = m_map.getPath(m_currentTurn->getCoordinates(), m_cursor.getCoordinates(), m_currentTurn->getStats().movement);
                path.first == AStar::PathType::Valid)
        {
            auto it = std::find_if(
                    m_units.begin()
                    , m_units.end()
                    , [&](const Unit& unit) { return &unit == m_currentTurn; });
            moveCharacter(*it, m_cursor.getCoordinates());
            advanceTurn();
        }
    }
}

void Board::cancel()
{
}
