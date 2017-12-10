#pragma once

#include <SFML/System/Vector2.hpp>

namespace JMath
{
    int maxInt();
    float maxFloat();

    int clamp(const int _original, const int _min, const int _max);
    float clampf(const float _original, const float _min, const float _max);

    float vector2Distance(const sf::Vector2f& _a, const sf::Vector2f& _b);
    float vector2DistanceSqr(const sf::Vector2f& _a, const sf::Vector2f& _b);

    float vector2Magnitude(const sf::Vector2f& _v);
    float vector2MagnitudeSqr(const sf::Vector2f& _v);

    sf::Vector2f vector2Normalized(const sf::Vector2f& _v);

}
