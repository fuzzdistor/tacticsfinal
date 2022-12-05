#include "tweener.hpp"
#include <cassert>

void Tweener::createTween(TweenFunction&& tweenFunction)
{
    m_tweenableFunctions.emplace_back(m_uniqueId, std::forward<TweenFunction>(tweenFunction));
}

void Tweener::update(sf::Time dt)
{
    std::vector<size_t> finishedTweens;

    for (auto& element : m_tweenableFunctions)
    {
        if (element.second(dt))
            finishedTweens.push_back(element.first);
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
                m_tweenableFunctions.begin()
                , m_tweenableFunctions.end()
                , [&] (const auto& tween)
                { return tween.first == id; });

        assert(it != m_tweenableFunctions.end());
        m_tweenableFunctions.erase(it);
    }
}

Tweener::Tweener()
    : m_uniqueId(0)
{
}
