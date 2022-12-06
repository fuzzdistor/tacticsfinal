#ifndef TF_TILEMARKERS_HPP
#define TF_TILEMARKERS_HPP

#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include <memory>

class TileMarkers
{
public:
    TileMarkers();
    void updateHighlightedTiles(const std::vector<sf::Vector2u>& tiles);
    void updateTargetTile(const sf::Vector2u& target);
    void updatePathMarkers(const std::vector<sf::Vector2u>& path);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    std::vector<sf::RectangleShape> m_tileRects;
    std::vector<sf::CircleShape> m_pathTrail;
    std::unique_ptr<sf::RectangleShape> m_targetTile;
};

#endif // TF_TILEMARKERS_HPP
