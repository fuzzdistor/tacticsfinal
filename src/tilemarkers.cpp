#include "tilemarkers.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/System/Vector2.hpp"

TileMarkers::TileMarkers()
    : m_tileRects()
    , m_pathTrail()
    , m_targetTile(std::make_unique<sf::RectangleShape>())
{
}


void TileMarkers::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (auto& rect : m_tileRects)
        target.draw(rect, states);
    for (auto& circle : m_pathTrail)
        target.draw(circle, states);
    target.draw(*m_targetTile, states);
}

void TileMarkers::updatePathMarkers(const std::vector<sf::Vector2u>& path)
{
    m_pathTrail.clear();
    for (const auto& pos : path)
    {
        sf::CircleShape circle;
        float radius = 1.f;
        m_pathTrail.emplace_back(radius);
        m_pathTrail.back().setFillColor(sf::Color::Yellow);
        m_pathTrail.back().setPosition(sf::Vector2f(pos * 8u) + sf::Vector2f(4.f, 4.f));
        m_pathTrail.back().setOrigin({radius, radius});
    }
    m_pathTrail.front().setFillColor(sf::Color::Cyan);
    m_pathTrail.front().setRadius(1.3f);
    m_pathTrail.back().setOrigin({1.3f, 1.3f});
}

void TileMarkers::updateHighlightedTiles(const std::vector<sf::Vector2u>& tiles)
{
    m_tileRects.clear();
    m_tileRects.reserve(tiles.size());
    sf::Vector2f size(7.8f, 7.8f);
    for (const auto& tile : tiles)
    {
        m_tileRects.emplace_back(size);
        m_tileRects.back().setFillColor(sf::Color(0x0000AA88));
        m_tileRects.back().setPosition(
                { static_cast<float>(tile.x * 8u) + 0.1f,
                static_cast<float>(tile.y * 8u) + 0.1f });
    }

}

void TileMarkers::updateTargetTile(const sf::Vector2u& target)
{
    sf::Vector2f size(7.8f, 7.8f);
    m_targetTile->setSize(size);
    m_targetTile->setFillColor(sf::Color(0x00AAAA88));
    m_targetTile->setPosition(
            { static_cast<float>(target.x * 8u) + 0.1f,
            static_cast<float>(target.y * 8u) + 0.1f });
}
