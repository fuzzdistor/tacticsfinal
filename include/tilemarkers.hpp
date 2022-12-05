#ifndef TF_TILEMARKERS_HPP
#define TF_TILEMARKERS_HPP

#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

class TileMarkers
{
public:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    void updateHighlightedTiles(const std::vector<sf::Vector2u>& tiles);
    void updatePathMarkers(const std::vector<sf::Vector2u>& path);

private:
    std::vector<sf::RectangleShape> m_tileRects;
    std::vector<sf::CircleShape> m_pathTrail;
};

#endif // TF_TILEMARKERS_HPP
