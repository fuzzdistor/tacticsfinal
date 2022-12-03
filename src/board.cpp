#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/System/Vector2.hpp"
#include "board.hpp"
#include "map.hpp"
#include "resourcemanager.hpp"
#include "utils.hpp"
#include <chrono>
#include <vector>

Board::Board(const MapData& data)
    : m_map(data)
    , m_playerTeam(6)
    , m_enemyTeam(6)
    , m_pathfinder(m_map)
{
    m_playerTeam.emplaceUnit(ResourcePack::getInstance().textures.get(fd::hash("Allied")));
    auto& enemy = m_enemyTeam.emplaceUnit(ResourcePack::getInstance().textures.get(fd::hash("Enemy")));
    m_playerTeam[0].setMovement(6);
    m_playerTeam[0].setPosition({5, 8});
    enemy.setMovement(6);
    enemy.setPosition({3, 3});

    auto& pack = ResourcePack::getInstance();

    m_cursor.sprite.setTexture(pack.textures.get(fd::hash("Cursor")));
    m_cursor.shape.setFillColor(sf::Color(0xAAAA00AA));

    pack.textures.load(fd::hash("Map1"), data.imagepath);
    m_sprite.setTexture(pack.textures.get(fd::hash("Map1")));

    updateHighlightedTiles();
}

template<typename T>
void Board::setEntityPosition(T& entity, const sf::Vector2u& position, Map::Terrain mask)
{
    // recibo un opcional de path. si existe el path lo recorro, sino nada.
    if (auto path = m_pathfinder.getPath(entity.getPosition(), position, entity.getMovement(), mask))
    {
        entity.setPosition(path.value().front());     // muevo la unidad a la posición de goal

        updatePathRects(*path);
        updateHighlightedTiles();
    }
}

void Board::updatePathRects(const std::vector<sf::Vector2u>& path)
{
    m_path_rects.clear();
    for (const auto& pos : path)
    {
        m_path_rects.emplace_back(sf::Vector2f(2.f, 2.f));
        m_path_rects.back().setFillColor(sf::Color::Yellow);
        m_path_rects.back().setPosition(sf::Vector2f(pos * 8u) + sf::Vector2f(3, 3));
    }
}

void Board::moveCharacter(const sf::Vector2u& position)
{
    setEntityPosition(m_playerTeam.at(0), position);
}

void Board::moveCursor(const sf::Vector2u& movement)
{
    m_cursor.position += movement;
}

void Board::setCursorPosition(const sf::Vector2u& position)
{
    m_cursor.position = position;
}

void Board::updateHighlightedTiles()
{
    const auto positions = m_map.getRangeRadius(m_playerTeam.at(0).getPosition(), m_playerTeam.at(0).getMovement());
    m_tile_rects.clear();
    m_tile_rects.reserve(positions.size());
    for (const auto& position : positions)
    {
        if (auto path = m_pathfinder.getPath(m_playerTeam.at(0).getPosition(), position, m_playerTeam.at(0).getMovement()))
        {
            sf::Vector2f size(7.8f, 7.8f);
            m_tile_rects.emplace_back(size);
            if (position == m_playerTeam.at(0).getPosition())
                m_tile_rects.back().setFillColor(sf::Color(0x33AAAA88));
            else
                m_tile_rects.back().setFillColor(sf::Color(0x0000AA88));
            m_tile_rects.back().setPosition(
                    { static_cast<float>(position.x * m_tile_width) + 0.1f,
                    static_cast<float>(position.y * m_tile_height) + 0.1f });
        }
    }
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
    for (const auto& rect : m_tile_rects)
        target.draw(rect, states);
    m_playerTeam.at(0).draw(target, states);
    states = sf::RenderStates();
    states.transform.translate(sf::Vector2f(m_cursor.position*8u));
    target.draw(m_cursor.shape, states);
    states.transform.translate(sf::Vector2f(0, -7.f));
    target.draw(m_cursor.sprite, states);
    states = sf::RenderStates();
    m_enemyTeam.at(0).draw(target, states);
    states = sf::RenderStates();
    for (const auto& rect : m_path_rects)
        target.draw(rect, states);
}

void Board::accept()
{
    //if (m_map.getTerrain(m_cursor.position) == Map::Terrain::Unit)
    moveCharacter(m_cursor.position);
}

void Board::cancel()
{
}

void Board::turnTick()
{
}
