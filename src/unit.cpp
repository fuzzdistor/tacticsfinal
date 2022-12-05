#include "unit.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Transformable.hpp"
#include "SFML/Graphics/Vertex.hpp"
#include "SFML/System/Vector2.hpp"
#include "tween.hpp"
#include "utils.hpp"
#include "tweener.hpp"

Unit::Unit(const sf::Texture& texture)
    : m_name()
    , m_coords()
    , m_position()
    , m_sprite(texture)
    , m_tween()
    , m_stats()
    , m_status(Status::None)
    , m_id()
{
}

void Unit::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform.translate(m_position);
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

void Unit::setPosition(const sf::Vector2u& position)
{
    m_tween.setTween(&m_position, m_position, sf::Vector2f(position)*8.f, sf::seconds(1.f));
    Tweener::getInstance().createTween([&](sf::Time dt)
            { m_tween.update(dt); return !m_tween.isActive(); });
    m_coords = position;
}

void Unit::setTexture(const sf::Texture& texture)
{
    m_sprite.setTexture(texture);
}

