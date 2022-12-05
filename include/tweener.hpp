#ifndef TF_TWEENER_HPP
#define TF_TWEENER_HPP

#include "tween.hpp"
#include <functional>
#include <type_traits>
#include <vector>
#include <memory>

using uint = unsigned int;

template<Tweenable T>
struct TweenWithCallback
{
    TweenWithCallback(size_t p_id, T&& propierty, T&& start, T&& end, sf::Time&& time);
    ~TweenWithCallback();
    TweenWithCallback(TweenWithCallback<T>&& other) = delete;
    TweenWithCallback(const TweenWithCallback<T>& other) = delete;
    TweenWithCallback& operator=(const TweenWithCallback<T>& other) = delete;

    Tween<T> tween;
    std::function<void()> callback;
    size_t id;
};

template<Tweenable T>
TweenWithCallback<T>::TweenWithCallback(size_t p_id, T&& propierty, T&& start, T&& end, sf::Time&& time)
    : tween(propierty, start, end, time)
    , callback()
    , id(p_id)
{
}

/* template<Tweenable T> */
/* TweenWithCallback<T>::TweenWithCallback(TweenWithCallback<T>&& other) */
/*     : tween(std::move(other.tween)) */
/*     , callback(std::move(other.callback)) */
/*     , id(std::move(other.id)) */
/* { */
/* } */

template<Tweenable T>
TweenWithCallback<T>::~TweenWithCallback()
{
    /* tween.finish(); */
    callback();
}

class Tweener
{
public:
    Tweener();
    template <Tweenable T>
    Tween<T>& createTween(T& propierty, T&& start, T&& end, sf::Time&& time);
    void update(sf::Time dt);

private:
    void clearFinishedTweens(const std::vector<size_t>& finishedTweens);

    std::vector<std::unique_ptr<TweenWithCallback<float>>> m_floattweens;
    std::vector<std::unique_ptr<TweenWithCallback<uint>>> m_uinttweens;
    size_t m_uniqueId;
};

template<Tweenable T>
Tween<T>& Tweener::createTween(T& propierty, T&& start, T&& end, sf::Time&& time)
{
    if (std::is_same_v<T, float>)
        return m_floattweens.emplace_back(
                m_uniqueId
                , propierty
                , std::forward<T&&>(start)
                , std::forward<T&&>(end)
                , std::forward<sf::Time&&>(time));
    if (std::is_same_v<T, uint>)
        return m_uinttweens.emplace_back(
                m_uniqueId
                , propierty
                , std::forward<T&&>(start)
                , std::forward<T&&>(end)
                , std::forward<sf::Time&&>(time));

    m_uniqueId++;
}

#endif // TF_TWEENER_HPP
