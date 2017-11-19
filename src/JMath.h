#pragma once

#include <SFML/System/Vector2.hpp>

namespace JMath
{
    float maxFloat();

    float clampf(const float _original, const float _min, const float _max);
    int clamp(const int _original, const int _min, const int _max);

    float Vector2Distance(const sf::Vector2f& _a, const sf::Vector2f& _b);
    float Vector2DistanceSqr(const sf::Vector2f& _a, const sf::Vector2f& _b);
}
