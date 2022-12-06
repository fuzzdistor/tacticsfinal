#ifndef TF_EASINGFUNCTIONS_HPP
#define TF_EASINGFUNCTIONS_HPP

#include <cmath>

enum class Easing
{
    Linear,
    Squared,
    SquareRoot,
    QuadEaseOut,
    SmoothStep
};

constexpr float linear(float t)
{
    return t;
}

constexpr float squared(float t)
{
    return t*t;
}

constexpr float squareroot(float t)
{
    return std::sqrt(t);
}

constexpr float quadraticeaseout(float t)
{
    return 1.f - (1.f - t) * (1.f - t);
}

constexpr float smoothstep(float t)
{
    float v1 = t*t;
    float v2 = 1.f - (1.f - t) * (1.f - t);
    return std::lerp(v1, v2, t);
}

#endif // TF_EASINGFUNCTIONS_HPP
