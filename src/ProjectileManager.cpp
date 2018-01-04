#include "ProjectileManager.h"
#include "AssetManager.h"
#include "EnemyDirector.h"
#include "JTime.h"
#include "JMath.h"


ProjectileManager::ProjectileManager(AssetManager& _asset_manager, EnemyDirector& _enemy_director)
    : enemy_director(_enemy_director)
{
    initBullets(_asset_manager);
}


void ProjectileManager::tick(GameData& _gd)
{
    for (auto& bullet : bullets)
    {
        if (!bullet.isVisible())
            continue;

        bullet.tick();

        if (enemy_director.killEnemyAtPos(bullet.getPosition()))
        {
            bullet.destroy();
            break;
        }
    }
}


void ProjectileManager::draw(sf::RenderWindow& _window)
{
    for (auto& laser : lasers)
    {
        if (!laser.isVisible())
            continue;

        laser.draw(_window);
    }

    for (auto& bullet : bullets)
    {
        if (!bullet.isVisible())
            continue;

        bullet.draw(_window);
    }
}


void ProjectileManager::requestLaser(const sf::Vector2f& _from, const sf::Vector2f& _to)
{
    for (auto& laser : lasers)
    {
        if (laser.isVisible())
            continue;

        laser.refresh(_from, _to);
        break;
    }
}


void ProjectileManager::requestBullet(const sf::Vector2f& _from, const sf::Vector2f& _to)
{
    for (auto& bullet : bullets)
    {
        if (bullet.isVisible())
            continue;

        bullet.refresh(_from, _to);
        break;
    }
}


ProjectileManager::TowerLaser::TowerLaser()
    : line(sf::LineStrip, 2)
    , line_color(sf::Color::Cyan)
    , visible_duration(0.1f)
    , draw_until_time(0)
{
    line[0].color = line_color;
    line[1].color = line_color;
}


void ProjectileManager::TowerLaser::draw(sf::RenderWindow& _window)
{
    _window.draw(line);
}


void ProjectileManager::TowerLaser::refresh(const sf::Vector2f& _from, const sf::Vector2f& _to)
{
    line[0].position = _from;
    line[1].position = _to;

    draw_until_time = JTime::getTime() + visible_duration;
}


bool ProjectileManager::TowerLaser::isVisible() const
{
    if (JTime::getTime() > draw_until_time)
        return false;

    return true;
}


ProjectileManager::TowerBullet::TowerBullet()
    : visible_duration(3)
    , draw_until_time(0)
{
}


void ProjectileManager::TowerBullet::setTexture(sf::Texture* _texture)
{
    sprite.setTexture(_texture);
}


void ProjectileManager::TowerBullet::setColor(const sf::Color& _color)
{
    sprite.setColor(_color);
}


void ProjectileManager::TowerBullet::tick()
{
    float dt = JTime::getDeltaTime();
    sprite.setPosition(sprite.getPosition() + (direction * dt * 150.0f));
}


void ProjectileManager::TowerBullet::draw(sf::RenderWindow& _window)
{
    sprite.draw(_window);
}


void ProjectileManager::TowerBullet::refresh(const sf::Vector2f& _from, const sf::Vector2f& _to)
{
    sprite.setPosition(_from);
    direction = JMath::vector2Normalized(_to - _from);

    draw_until_time = JTime::getTime() + visible_duration;
}


bool ProjectileManager::TowerBullet::isVisible() const
{
    if (JTime::getTime() > draw_until_time)
        return false;

    return true;
}


void ProjectileManager::TowerBullet::destroy()
{
    draw_until_time = 0;
}


const sf::Vector2f& ProjectileManager::TowerBullet::getPosition() const
{
    return sprite.getPosition();
}


void ProjectileManager::initBullets(AssetManager& _asset_manager)
{
    auto* texture = _asset_manager.loadTexture(BULLET_SPRITE);
    for (auto& bullet : bullets)
    {
        bullet.setTexture(texture);
        bullet.setColor(sf::Color::Yellow);
    }
}
