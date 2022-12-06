#include "tweener.hpp"
#include "utils.hpp"
#include <cassert>

Tweener::Tweener()
    : m_uniqueId(0)
{
}

void Tweener::createTween(TweenFunction&& tweenFunction)
{
    m_tweens.emplace_back(m_uniqueId, std::forward<TweenFunction>(tweenFunction));
    m_uniqueId++;
}

void Tweener::update(sf::Time dt)
{
    std::vector<size_t> finishedTweens;

    for (size_t index = 0; index < m_tweens.size(); index++)
    {
        if (m_tweens[index].second(dt))
        {
            finishedTweens.push_back(m_tweens[index].first);
        }
    }

    clearFinishedTweens(finishedTweens);
}

Tweener& Tweener::getInstance()
{
    static Tweener instance;
    return instance;
}

void Tweener::clearFinishedTweens(const std::vector<size_t>& finishedTweens)
{
    for (size_t id : finishedTweens)
    {
        auto it = std::find_if(
                m_tweens.begin()
                , m_tweens.end()
                , [&] (const auto& tween)
                { return tween.first == id; });

        assert(it != m_tweens.end());
        m_tweens.erase(it);
    }
}

