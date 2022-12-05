#ifndef TF_TWEEN_HPP
#define TF_TWEEN_HPP

#include "SFML/System/Time.hpp"
#include "SFML/System/Vector2.hpp"
#include <cmath>
#include <type_traits>

template <typename T>
concept Tweenable = requires
{
    std::is_arithmetic_v<float>
        || std::is_same_v<T, sf::Vector2f>;
};

template <Tweenable T>
class Tween
{
public:
    Tween();
    Tween(T* propierty, T start, T end, sf::Time time);

    void setTween(T* propierty, T start, T end, sf::Time time);

    void update(sf::Time dt);
    void update(sf::Time dt) requires (std::is_same_v<T, sf::Vector2f>);
    void finish();
    bool isActive() const;

private:
    sf::Time m_time;
    sf::Time m_t;
    T m_start;
    T m_end;
    T* m_propierty;
    bool m_isActive;
};

template<Tweenable T>
Tween<T>::Tween()
    : m_time()
    , m_t(sf::Time::Zero)
    , m_start()
    , m_end()
    , m_propierty()
    , m_isActive(false)
{
}

template<Tweenable T>
bool Tween<T>::isActive() const
{
    return m_isActive;
}

template<Tweenable T>
void Tween<T>::setTween(T* propierty, T start, T end, sf::Time time)
{
    m_time = time;
    m_start = start;
    m_end = end;
    m_propierty = propierty;
    m_t = sf::Time::Zero;
    m_isActive = true;
}

template<Tweenable T>
Tween<T>::Tween(T* propierty, T start, T end, sf::Time time)
    : m_time(time)
    , m_t(sf::Time::Zero)
    , m_start(start)
    , m_end(end)
    , m_propierty(propierty)
    , m_isActive(true)
{
}

template<Tweenable T>
void Tween<T>::finish()
{
    m_propierty = m_end;
}

template<Tweenable T>
void Tween<T>::update(sf::Time dt)
{
    m_t += dt;
    *m_propierty = std::lerp(m_start, m_end, m_t / m_time);
    m_isActive = m_t < m_time;
}

template<Tweenable T>
void Tween<T>::update(sf::Time dt) requires (std::is_same_v<T, sf::Vector2f>)
{
    m_t += dt;
    m_propierty->x = std::lerp(m_start.x, m_end.x, m_t / m_time);
    m_propierty->y = std::lerp(m_start.y, m_end.y, m_t / m_time);
    m_isActive = m_t < m_time;
}

#endif // TF_TWEEN_HPP

