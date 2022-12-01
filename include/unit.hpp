#ifndef TF_UNIT_HPP
#define TF_UNIT_HPP

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/System/NonCopyable.hpp"
#include "stats.hpp"
#include "utils.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

enum Status
{
    Quick,
    Slow,
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
    sf::Vector2u getPosition() const;
    Status getStatus() const;
    void setPosition(const sf::Vector2u& movement);
    void draw(sf::RenderTarget& target, sf::RenderStates state) const final;

private:

    uint m_id;
    sf::Vector2u m_position;
    sf::Sprite m_sprite;
    Stats m_stats;
};

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

#endif // TF_UNIT_HPP

