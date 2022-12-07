#ifndef TF_TWEENER_HPP
#define TF_TWEENER_HPP

#include "tween.hpp"
#include <functional>
#include <type_traits>
#include <vector>
#include <memory>

using uint = unsigned int;

class Tweener
{
public:
    typedef std::function<bool(sf::Time)> TweenFunction;
    static Tweener& getInstance();
    void createTween(TweenFunction&& tweenFunction, bool isBlocking = true);
    void update(sf::Time dt);
    bool isActive() const;

private:
    Tweener();
    void clearFinishedTweens(const std::vector<size_t>& finishedTweens);

    std::vector<std::pair<size_t, TweenFunction>> m_tweens;
    std::vector<size_t> m_blockingTweenIds;
    size_t m_uniqueId;
};

#endif // TF_TWEENER_HPP
