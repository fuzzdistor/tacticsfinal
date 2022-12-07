#include "cursor.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "easingfunctions.hpp"
#include "resourcemanager.hpp"
#include "tween.hpp"
#include "tweener.hpp"
#include "utils.hpp"

Cursor::Cursor()
    : m_coords()
    , m_position()
    , m_sprite()
    , m_tileHighlight()
{
    m_sprite.setTexture(ResourcePack::getInstance().textures.get(fd::hash("Cursor")));
    m_sprite.setOrigin(0.f, 6.f);
    m_tileHighlight.setFillColor(sf::Color(0xAAAA00AA));
    m_tileHighlight.setSize(sf::Vector2f(7.8f, 7.8f));
    m_tileHighlight.setPosition(
            static_cast<float>(m_coords.x * 8u) + 0.1f
            , static_cast<float>(m_coords.y * 8u) + 0.1f);
}


sf::Vector2u Cursor::getCoordinates() const
{
    return m_coords;
}

void Cursor::setCoordinates(const sf::Vector2u& coords)
{
    m_coords = coords;
    m_position = sf::Vector2f(coords*8u);
}


void Cursor::tweenPosition(const sf::Vector2u& position)
{
    m_coords = position;
    Tween tween(&m_position, m_position, sf::Vector2f(m_coords*8u), sf::seconds(0.2f), Easing::QuadEaseOut);
    Tweener::getInstance().createTween([tween = tween](sf::Time dt) mutable
            {
                tween.update(dt);
                return !tween.isActive();
            }, false);
    m_tileHighlight.setPosition(sf::Vector2f(m_coords * 8u));
}

void Cursor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_tileHighlight, states);
    states.transform.translate(m_position);
    target.draw(m_sprite, states);
}
