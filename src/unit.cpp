#include "unit.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Transformable.hpp"
#include "SFML/Graphics/Vertex.hpp"
#include "SFML/System/Vector2.hpp"
#include "easingfunctions.hpp"
#include "tween.hpp"
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
    , m_playerControlled(false)
{
}

Unit::Unit(const sf::Texture& texture, const std::string& name, const Stats& stats)
    : m_name(name)
    , m_coords()
    , m_position()
    , m_sprite(texture)
    , m_stats(stats)
    , m_id()
    , m_playerControlled(false)
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

sf::Vector2u Unit::getCoordinates() const
{
    return m_coords;
}

void Unit::tweenPosition(const sf::Vector2u& position)
{
    // uso lambdas para llevar el tween y su estado sin tener que almacenarlo
    // en el objeto de unit. una vez que se finaliza el tween y la funcion
    // devuelve false, el Tweener la quita de su vector y el estado desaparece.
    Tween positionTween(&m_position, m_position, sf::Vector2f(position*8u), sf::seconds(0.2f));
    Tweener::getInstance().createTween([t = positionTween](sf::Time dt) mutable
            { t.update(dt); return !t.isActive(); });
    m_coords = position;
}

void Unit::tweenPath(std::vector<sf::Vector2u>& path)
{
    if (path.size() == 0)
        return;

    Tween pathTween(&m_position, m_position, sf::Vector2f(path.back()*8u), sf::seconds(0.2f), Easing::Type::SmoothStep);
    m_coords = path.front();
    path.pop_back();
    Tweener::getInstance().createTween([path = path, tween = pathTween, this](sf::Time dt) mutable
            {
            tween.update(dt);
            if (!tween.isActive())
                tweenPath(path);
            return !tween.isActive();
            });
}

void Unit::markDead()
{
    m_sprite.setColor(sf::Color::Black);
    m_status = Status::Dead;
}


void Unit::setCoordinates(const sf::Vector2u& position)
{
    m_position = sf::Vector2f(position*8u);
    m_coords = position;
}

void Unit::setTexture(const sf::Texture& texture)
{
    m_sprite.setTexture(texture);
}

