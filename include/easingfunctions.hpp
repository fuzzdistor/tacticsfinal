#ifndef TF_EASINGFUNCTIONS_HPP
#define TF_EASINGFUNCTIONS_HPP

#include <cmath>

namespace Easing
{
enum class Type
{
    Linear,
    Squared,
    SquareRoot,
    QuadEaseOut,
    SmoothStep
};

inline float linear(float t)
{
    return t;
}

inline float squared(float t)
{
    return t*t;
}

inline float squareroot(float t)
{
#ifdef __GNUC__
    return sqrtf(t);
#else
    return std::sqrtf(t);
#endif

}

inline float quadraticeaseout(float t)
{
    return 1.f - (1.f - t) * (1.f - t);
}

inline float smoothstep(float t)
{
    float v1 = t*t;
    float v2 = 1.f - (1.f - t) * (1.f - t);
    return std::lerp(v1, v2, t);
}
}
#endif // TF_EASINGFUNCTIONS_HPP
