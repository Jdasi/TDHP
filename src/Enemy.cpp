#include "Enemy.h"
#include "JTime.h"
#include "JMath.h"
#include "GDebugFlags.h"


Enemy::Enemy()
    : path_index(0)
    , type(nullptr)
    , speed_modifier(1)
{
    initHealthBar();
}


EnemyType* Enemy::getType()
{
    return type;
}


void Enemy::setType(EnemyType& _type)
{
    type = &_type;

    setMaxHealth(_type.max_health);
    setTexture(_type.texture);
}


void Enemy::tick()
{
    scheduler.update();

    if (path_index >= path.getNumPoints())
        return;

    // updateTileIndex(); -- INFO UNUSED.

    auto& waypoint = path.getWaypoint(path_index);
    auto& pos = getPosition();

    float remaining_dist_sqr = JMath::vector2DistanceSqr(pos, waypoint.pos);

    if (pos == waypoint.pos || remaining_dist_sqr <= 1)
    {
        nextWaypoint();
    }
    else
    {
        moveToWaypoint(waypoint, pos, remaining_dist_sqr);
    }
}


void Enemy::draw(sf::RenderWindow& _window)
{
    TDSprite::draw(_window);

    if (GDebugFlags::draw_paths)
        path.draw(_window, getPosition(), path_index);

    health_bar.draw(_window);
}


void Enemy::setPath(const LevelPath& _path)
{
    path_index = 0;
    path = _path;
}


void Enemy::boostHealth(const int _modifier, const float _duration)
{
    if (type == nullptr)
        return;

    scheduler.cancelInvoke("BoostHealth");

    setMaxHealth(type->max_health * _modifier);
    health_bar.setBarColor(sf::Color::Red);

    scheduler.invoke([this]()
    {
        resetHealth();
    }, _duration, "BoostHealth");
}


void Enemy::boostSpeed(const float _modifier, const float _duration)
{
    if (type == nullptr)
        return;

    scheduler.cancelInvoke("BoostSpeed");

    speed_modifier = _modifier;
    setColor(sf::Color::Yellow);

    scheduler.invoke([this]()
    {
        resetSpeed();
    }, _duration, "BoostSpeed");
}


void Enemy::onSpawn()
{
    if (type == nullptr)
    {
        killQuiet();
        return;
    }

    resetHealth();
    resetSpeed();

    health_bar.updateValuePercentage(getHealthPercentage());
    scheduler.cancelInvokes();
}


void Enemy::onDamage(TowerType* _attacker_type)
{
    health_bar.updateValuePercentage(getHealthPercentage());
}


void Enemy::onDeath(TowerType* _killer_type)
{
    for (auto& listener : listeners)
    {
        listener->onDeath(getPosition(), _killer_type);
    }
}


void Enemy::onSetPosition()
{
    health_bar.updatePosition(getPosition());
}


void Enemy::initHealthBar()
{
    float width = getLevelTileWidth();
    float height = getLevelTileHeight();

    // Base health bar size on TDSprite size (aka tile size).
    health_bar.configure({ width * 0.9f, height * 0.1f }, -height * 0.5f);
    health_bar.setBarColor(sf::Color::Green);
}


void Enemy::updateTileIndex()
{
    int size_x = getLevelSizeX();

    float width = getLevelTileWidth();
    float height = getLevelTileHeight();

    int tile_index = JHelper::posToTileIndex(getPosition(), width, height, size_x);
    setTileIndex(tile_index);

    coords = JHelper::calculateCoords(tile_index, size_x);
}


void Enemy::nextWaypoint()
{
    ++path_index;

    if (path_index >= path.getNumPoints())
    {
        for (auto& listener : listeners)
        {
            listener->onPathComplete(*this);
        }
    }
}

void Enemy::moveToWaypoint(const Waypoint& _waypoint, const sf::Vector2f& _pos,
    float _remaining_dist_sqr)
{
    sf::Vector2f dir = JMath::vector2Normalized(_waypoint.pos - _pos);
    dir *= type->speed * speed_modifier * JTime::getDeltaTime();

    // Prevent enemy from overshooting the waypoint.
    if (JMath::vector2MagnitudeSqr(dir) >= _remaining_dist_sqr)
    {
        setPosition(_waypoint.pos);
    }
    else
    {
        setPosition(_pos + dir);
    }
}


void Enemy::resetHealth()
{
    setMaxHealth(type->max_health);
    health_bar.setBarColor(sf::Color::Green);
}


void Enemy::resetSpeed()
{
    speed_modifier = 1;
    setColor(sf::Color::White);
}
