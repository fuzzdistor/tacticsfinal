#include "unit.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Transformable.hpp"
#include "SFML/Graphics/Vertex.hpp"
#include "utils.hpp"
#include "tweener.hpp"

Unit::Unit(const sf::Texture& texture)
    : m_name()
    , m_coords()
    , m_position()
    , m_sprite(texture)
    , m_stats()
    , m_status(Status::None)
    , m_id()
{
}

void Unit::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform.translate(sf::Vector2f(m_coords * 8u));
    target.draw(m_sprite, states);
}

const std::string& Unit::getName() const
{
    return m_name;
}

void Unit::setName(const std::string& name)
{
    m_name = name;
}

Status Unit::getStatus() const
{
    return m_status;
}


sf::Vector2u Unit::getPosition() const
{
    return m_coords;
}

void Unit::setPosition(const sf::Vector2u& movement)
{
    Tweener::getInstance().createTween([&](sf::Time dt)
            { D("HELLO FROM TWEEN!"); return true;});
    m_coords = movement;
}

void Unit::setTexture(const sf::Texture& texture)
{
    m_sprite.setTexture(texture);
}

