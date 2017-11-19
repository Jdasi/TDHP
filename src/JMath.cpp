#include <cmath>
#include <limits>

#include "JMath.h"


float JMath::maxFloat()
{
    return std::numeric_limits<float>::max();
}


float JMath::clampf(const float _original, const float _min, const float _max)
{
    if (_original < _min)
        return _min;

    if (_original > _max)
        return _max;

    return _original;
}


int JMath::clamp(const int _original, const int _min, const int _max)
{
    if (_original < _min)
        return _min;

    if (_original > _max)
        return _max;

    return _original;
}


float JMath::Vector2Distance(const sf::Vector2f& _a, const sf::Vector2f& _b)
{
    sf::Vector2f diff = _a - _b;
    return sqrtf((diff.x * diff.x) + (diff.y * diff.y));
}


float JMath::Vector2DistanceSqr(const sf::Vector2f& _a, const sf::Vector2f& _b)
{
    sf::Vector2f diff = _a - _b;
    return (diff.x * diff.x) + (diff.y * diff.y);
}
