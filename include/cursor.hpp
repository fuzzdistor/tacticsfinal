#ifndef TF_CURSOR_HPP
#define TF_CURSOR_HPP

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Sprite.hpp"

class Cursor
{
public:
    Cursor();

    void tweenPosition(const sf::Vector2u& position);
    void setCoordinates(const sf::Vector2u& coords);
    sf::Vector2u getCoordinates() const;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
    sf::Vector2u m_coords;
    sf::Vector2f m_position;
    sf::Sprite m_sprite;
    sf::RectangleShape m_tileHighlight;
};

#endif // TF_CURSOR_HPP
