#ifndef TF_TWEEN_HPP
#define TF_TWEEN_HPP

#include "SFML/System/Time.hpp"
#include <cmath>
#include <type_traits>

template <typename T>
concept Tweenable = requires
{
    std::is_arithmetic_v<float>;
};

template <Tweenable T>
class Tween
{
public:
    Tween(T* propierty, T start, T end, sf::Time time);
    Tween(const Tween<T>&) = delete;
    Tween(Tween<T>&&) = delete;

    bool update(sf::Time dt);
    /* void finish(); */

private:
    sf::Time m_time;
    sf::Time m_t;
    T m_start;
    T m_end;
    T* m_propierty;
};

template<Tweenable T>
Tween<T>::Tween(T* propierty, T start, T end, sf::Time time)
    : m_time(time)
    , m_t(sf::Time::Zero)
    , m_start(start)
    , m_end(end)
    , m_propierty(propierty)
{
    propierty = start;
}

/* template<Tweenable T> */
/* void Tween<T>::finish() */
/* { */
/*     m_propierty = m_end; */
/* } */

template<Tweenable T>
bool Tween<T>::update(sf::Time dt)
{
    m_t += dt;
    *m_propierty = std::lerp(m_start, m_end, m_t / m_time);
    return m_t >= m_time;
}

#endif // TF_TWEEN_HPP

