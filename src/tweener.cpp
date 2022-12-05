#include "tweener.hpp"
#include <cassert>

void Tweener::update(sf::Time dt)
{
    std::vector<size_t> finishedTweens;

    for (auto& twc : m_floattweens)
    {
        if(twc->tween.update(dt))
            finishedTweens.push_back(twc->id);
    }

    for (auto& twc : m_uinttweens)
    {
        if(twc->tween.update(dt))
            finishedTweens.push_back(twc->id);
    }

    clearFinishedTweens(finishedTweens);
}

void Tweener::clearFinishedTweens(const std::vector<size_t>& finishedTweens)
{
    for (size_t id : finishedTweens)
    {
        {
        auto it = std::remove_if(m_floattweens.begin(), m_floattweens.end(), [&]
                (const auto& tween) { return tween->id == id; });

        if (it != m_floattweens.end())
            m_floattweens.erase(it);
        }

        {
        auto it = std::find_if(m_uinttweens.begin(), m_uinttweens.end(), [&]
                (const auto& tween) { return tween->id == id; });

        if (it != m_uinttweens.end())
            m_uinttweens.erase(it);
        }
    }
}

Tweener::Tweener()
    : m_uniqueId(0)
{
}
