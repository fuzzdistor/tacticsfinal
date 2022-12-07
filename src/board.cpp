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
    , m_aliveUnits()
    , m_combatManager(m_map, m_units)
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
                    , [&](uint id){ reportDeath(id); }
                    );
            unit.setCoordinates(coordinates[i]);
            unit.setPlayerControlled(data_playercontrolled[i]);
            unit.setFaction(faction);
            unit.setId(static_cast<uint>(i));
            m_aliveUnits.emplace_back(unit.getId(), data_playercontrolled[i]);
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

void Board::reportDeath(uint id)
{
    // FIXME cambiar numeros magicos de faccion a enums
    std::erase_if(m_aliveUnits, [id=id](const auto& pair){ return pair.first == id; });
    auto it = std::find_if(m_aliveUnits.begin(), m_aliveUnits.end(), [](const auto& pair) { return pair.second == 1u; });
    if (it == m_aliveUnits.end())
        m_reportBattleResults(false);
    auto it2 = std::find_if(m_aliveUnits.begin(), m_aliveUnits.end(), [](const auto& pair) { return pair.second == 2u; });
    if (it2 == m_aliveUnits.end())
        m_reportBattleResults(true);
}

void Board::registerBattleResultCallback(std::function<void(bool)> callback)
{
    m_reportBattleResults = callback;
}

void Board::advanceTurn(TurnManager::ActionTaken action)
{
    // usar const_cast esta suuuuper mal visto, pero lo estoy usando
    // bien, para poder tomar de nuevo una referencia a un objeto
    // que a primeras no es const a traves de un const unit*. Esto
    // probablemente esconde un design flaw con mi programa, y debería
    // intentar refactorear el codigo pero por el momento va a
    // tener que servir. FIXME
    m_turnManager.takeCtFromUnit(const_cast<Unit&>(*m_currentTurn), action);

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
        // usar const_cast esta suuuuper mal visto, pero lo estoy usando
        // bien, para poder tomar de nuevo una referencia a un objeto
        // que a primeras no es const a traves de un const unit*. Esto
        // probablemente esconde un design flaw con mi programa, y debería
        // intentar refactorear el codigo pero por el momento va a
        // tener que servir. FIXME
        m_ai.takeTurn(const_cast<Unit&>(*m_currentTurn));
    }

    imguiWidget(*this);
}

void imguiWidget(Board& board)
{
    const Stats& s = board.m_currentTurn->getStats();
    ImGui::Begin("Current Turn");
    ImGui::Text("Unit: %s", board.m_currentTurn->getName().c_str());
    ImGui::Text("HP: %d/%d", s.healthPoints, s.maxHealthPoints);
    ImGui::Text("MP: %d/%d", s.magicPoints, s.maxMagicPoints);
    ImGui::Text("Lvl/Exp: %d/%d", s.level, s.experiencePoints);
    // TODO GUI
    ImGui::BeginChild("ASD");
    if (ImGui::Button("LABEL", ImVec2(100, 20)))
    {
    }
    ImGui::EndChild();
    ImGui::Begin("Info");

    auto it = std::find_if(board.m_units.begin(), board.m_units.end(), [&](const Unit& unit)
             { return unit.getCoordinates() == board.m_cursor.getCoordinates(); });
    if (it != board.m_units.end())
    {
        const Stats& u = it->getStats();
        ImGui::Text("Unit: %s", it->getName().c_str());
        ImGui::Text("HP: %d/%d", u.healthPoints, u.maxHealthPoints);
        ImGui::Text("MP: %d/%d", u.magicPoints, u.maxMagicPoints);
        ImGui::Text("Lvl/Exp: %d/%d", u.level, u.experiencePoints);
    }
    ImGui::End();
    //
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
    if (position.x > m_map.getSize().x || position.y > m_map.getSize().y)
        return;
    m_cursor.setCoordinates(position);
}

void Board::moveCursor(const sf::Vector2u& movement)
{
    if (!m_currentTurn->isPlayerControlled())
        return;
    auto position = m_cursor.getCoordinates() + movement;
    if (position.x > m_map.getSize().x - 1 || position.y > m_map.getSize().y - 1)
        return;
    m_cursor.tweenPosition(m_cursor.getCoordinates() + movement);
}

void Board::accept()
{
    if (!m_tweener.isActive() && m_currentTurn->isPlayerControlled())
    {
        // si las coordenadas son válidas avanza sino espera una coordenada valida
        if (auto path = m_map.getPath(m_currentTurn->getCoordinates(), m_cursor.getCoordinates(), m_currentTurn->getStats().movement);
                path.first == AStar::PathType::Valid)
        {
            // usar const_cast esta suuuuper mal visto, pero lo estoy usando
            // bien, para poder tomar de nuevo una referencia a un objeto
            // que a primeras no es const a traves de un const unit*. Esto
            // probablemente esconde un design flaw con mi programa, y debería
            // intentar refactorear el codigo pero por el momento va a
            // tener que servir. FIXME
            auto& character = const_cast<Unit&>(*m_currentTurn);
            moveCharacter(character, m_cursor.getCoordinates());

            // trata de atacar y avanza el turno acorde
            if (m_combatManager.tryAttack(character))
                advanceTurn(TurnManager::ActionTaken::MovedAndAction);
            else
                advanceTurn(TurnManager::ActionTaken::Moved);
        }
    }
}

void Board::cancel()
{
}

