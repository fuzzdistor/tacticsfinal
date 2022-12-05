#ifndef TF_UNIT_HPP
#define TF_UNIT_HPP

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/System/NonCopyable.hpp"
#include "stats.hpp"
#include "tween.hpp"
#include "utils.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

enum Status
{
    Slow,
    Haste,
    None,
};

class Unit : public sf::Drawable
{
public:
    Unit() = delete;

    explicit Unit(const sf::Texture& texture);

    void setTexture(const sf::Texture& texture);
    constexpr uint getId() const;
    constexpr uint getMovement() const;
    constexpr void setMovement(uint movement);
    constexpr void setAwareness(uint awareness);
    constexpr void setSpeed(uint speed);
    const std::string& getName() const;
    sf::Vector2u getPosition() const;
    Status getStatus() const;
    void setPosition(const sf::Vector2u& movement);
    void setName(const std::string& name);
    void draw(sf::RenderTarget& target, sf::RenderStates state) const final;
    constexpr const Stats& getStats() const;

private:

    std::string m_name;
    sf::Vector2u m_coords;
    sf::Vector2f m_position;
    sf::Sprite m_sprite;
    Stats m_stats;
    Status m_status;
    uint m_id;
};

constexpr const Stats& Unit::getStats() const
{
    return m_stats;
}
constexpr uint Unit::getId() const
{
    return m_id;
}

constexpr uint Unit::getMovement() const
{
    return m_stats.movement;
}

constexpr void Unit::setMovement(uint movement)
{
    m_stats.movement = movement;
}
constexpr void Unit::setAwareness(uint awareness)
{
    m_stats.awareness = awareness;
}

constexpr void Unit::setSpeed(uint speed)
{
    m_stats.speed = speed;
}

#endif // TF_UNIT_HPP

