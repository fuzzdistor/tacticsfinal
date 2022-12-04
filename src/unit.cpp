#include "unit.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Transformable.hpp"
#include "SFML/Graphics/Vertex.hpp"
#include "utils.hpp"

Unit::Unit(const sf::Texture& texture)
    : m_id()
    , m_position()
    , m_sprite(texture)
    , m_stats()
    , m_status(Status::None)
{
}

void Unit::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform.translate(sf::Vector2f(m_position * 8u));
    target.draw(m_sprite, states);
}

Status Unit::getStatus() const
{
    return m_status;
}


sf::Vector2u Unit::getPosition() const
{
    return m_position;
}

void Unit::setPosition(const sf::Vector2u& movement)
{
    m_position = movement;
}

void Unit::setTexture(const sf::Texture& texture)
{
    m_sprite.setTexture(texture);
}

