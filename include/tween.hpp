#ifndef TF_TWEEN_HPP
#define TF_TWEEN_HPP

#include "SFML/System/Time.hpp"
#include "SFML/System/Vector2.hpp"
#include "easingfunctions.hpp"
#include <cmath>
#include <functional>
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
    Tween(T* propierty, T start, T end, sf::Time time, Easing::Type easing = Easing::Type::Linear);

    void setTween(T* propierty, T start, T end, sf::Time time, Easing::Type easing = Easing::Type::Linear);

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
    std::function<float(float)> m_easingFunction;
    bool m_isActive;
};

template<Tweenable T>
Tween<T>::Tween()
    : m_time(sf::Time::Zero)
    , m_t(sf::Time::Zero)
    , m_start()
    , m_end()
    , m_propierty()
    , m_easingFunction(&Easing::linear)
    , m_isActive(false)
{
}

template<Tweenable T>
bool Tween<T>::isActive() const
{
    return m_isActive;
}

inline std::function<float(float)> getEasingFunc(Easing::Type easing)
{
    switch (easing)
    {
    case Easing::Type::Linear:
        return &Easing::linear;
    case Easing::Type::Squared:
        return &Easing::squared;
    case Easing::Type::SquareRoot:
        return &Easing::squareroot;
    case Easing::Type::QuadEaseOut:
        return &Easing::quadraticeaseout;
    case Easing::Type::SmoothStep:
        return &Easing::smoothstep;
    }
    return &Easing::linear;
}

template<Tweenable T>
void Tween<T>::setTween(T* propierty, T start, T end, sf::Time time, Easing::Type easing)
{
    m_time = time;
    m_start = start;
    m_end = end;
    m_propierty = propierty;
    m_t = sf::Time::Zero;
    m_easingFunction = getEasingFunc(easing);
    m_isActive = true;
}

template<Tweenable T>
Tween<T>::Tween(T* propierty, T start, T end, sf::Time time, Easing::Type easing)
    : m_time(time)
    , m_t(sf::Time::Zero)
    , m_start(start)
    , m_end(end)
    , m_propierty(propierty)
    , m_easingFunction(getEasingFunc(easing))
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
    *m_propierty = std::lerp(m_start, m_end, m_easingFunction(m_t / m_time));
    m_isActive = m_t < m_time;
}

template<Tweenable T>
void Tween<T>::update(sf::Time dt) requires (std::is_same_v<T, sf::Vector2f>)
{
    m_t += dt;
    m_propierty->x = std::lerp(m_start.x, m_end.x, m_easingFunction(m_t / m_time));
    m_propierty->y = std::lerp(m_start.y, m_end.y, m_easingFunction(m_t / m_time));
    m_isActive = m_t < m_time;
}

#endif // TF_TWEEN_HPP

