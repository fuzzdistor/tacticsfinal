#include "tilemarkers.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/System/Vector2.hpp"


void TileMarkers::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (auto& rect : m_tileRects)
        target.draw(rect, states);
    for (auto& circle : m_pathTrail)
        target.draw(circle, states);
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
}

void TileMarkers::updateHighlightedTiles(const std::vector<sf::Vector2u>& tiles)
{
    m_tileRects.clear();
    m_tileRects.reserve(tiles.size());
    for (const auto& tile : tiles)
    {
        sf::Vector2f size(7.8f, 7.8f);
        m_tileRects.emplace_back(size);
        m_tileRects.back().setFillColor(sf::Color(0x0000AA88));
        m_tileRects.back().setPosition(
                { static_cast<float>(tile.x * 8u) + 0.1f,
                static_cast<float>(tile.y * 8u) + 0.1f });
    }
}
