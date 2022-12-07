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

enum class Status
{
    Slow,
    Haste,
    None,
    Dead,
};

class Unit : public sf::Drawable
{
public:
    enum class State
    {
        Waiting,
        Moving,
        Moved,
        Acted,
    };

    Unit() = delete;
    // TODO  evitar que sean copiables

    explicit Unit(const sf::Texture& texture);
    Unit(const sf::Texture& texture, const std::string& name, const Stats& stats);

    void setTexture(const sf::Texture& texture);
    constexpr uint getId() const;
    constexpr void setMovement(uint movement);
    constexpr void setAwareness(uint awareness);
    constexpr void setFaction(uint faction);
    constexpr uint getFaction() const;
    constexpr void setSpeed(uint speed);
    const std::string& getName() const;
    sf::Vector2u getCoordinates() const;
    Status getStatus() const;
    constexpr const Stats& getStats() const;
    constexpr Stats& getStats();
    constexpr bool isDead() const;
    void markDead();
    constexpr bool isPlayerControlled() const;
    constexpr void setPlayerControlled(bool state);
    void setCoordinates(const sf::Vector2u& position);
    void setName(const std::string& name);
    void draw(sf::RenderTarget& target, sf::RenderStates state) const final;
    void tweenPosition(const sf::Vector2u& position);
    void tweenPath(std::vector<sf::Vector2u>& path);

private:

    std::string m_name;
    sf::Vector2u m_coords;
    sf::Vector2f m_position;
    sf::Sprite m_sprite;
    Stats m_stats;
    Status m_status;
    uint m_id;
    uint m_faction;
    bool m_playerControlled;
    State m_state;
};

constexpr const Stats& Unit::getStats() const
{
    return m_stats;
}

constexpr Stats& Unit::getStats()
{
    return m_stats;
}

constexpr uint Unit::getId() const
{
    return m_id;
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
constexpr bool Unit::isPlayerControlled() const
{
    return m_playerControlled;
}

constexpr void Unit::setPlayerControlled(bool state)
{
    m_playerControlled = state;
}

constexpr void Unit::setFaction(uint faction)
{
    m_faction = faction;
}

constexpr uint Unit::getFaction() const
{
    return m_faction;
}

constexpr bool Unit::isDead() const
{
    return m_status == Status::Dead;
}

#endif // TF_UNIT_HPP

