#include <cmath>
#include <limits>

#include "JMath.h"


int JMath::maxInt()
{
    return std::numeric_limits<int>::max();
}


float JMath::maxFloat()
{
    return std::numeric_limits<float>::max();
}


int JMath::clamp(const int _original, const int _min, const int _max)
{
    if (_original < _min)
        return _min;

    if (_original > _max)
        return _max;

    return _original;
}


float JMath::clampf(const float _original, const float _min, const float _max)
{
    if (_original < _min)
        return _min;

    if (_original > _max)
        return _max;

    return _original;
}


int JMath::roundToNearestMultiple(const int _val, const int _multiple)
{
    return ((_val + _multiple / 2) / _multiple) * _multiple;
}


float JMath::vector2Distance(const sf::Vector2f& _a, const sf::Vector2f& _b)
{
    sf::Vector2f diff = _a - _b;
    return sqrtf((diff.x * diff.x) + (diff.y * diff.y));
}


float JMath::vector2DistanceSqr(const sf::Vector2f& _a, const sf::Vector2f& _b)
{
    sf::Vector2f diff = _a - _b;
    return (diff.x * diff.x) + (diff.y * diff.y);
}


float JMath::vector2Magnitude(const sf::Vector2f& _v)
{
    return sqrtf(_v.x * _v.x + _v.y * _v.y);
}


float JMath::vector2MagnitudeSqr(const sf::Vector2f& _v)
{
    return _v.x * _v.x + _v.y * _v.y;
}


sf::Vector2f JMath::vector2Normalized(const sf::Vector2f& _v)
{
    sf::Vector2f v;

    float mag = 1.0f / JMath::vector2Magnitude(_v);

    v.x = _v.x * mag;
    v.y = _v.y * mag;

    return v;
}
