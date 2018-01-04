#pragma once

#include <array>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "IProjectileManager.h"
#include "TDSprite.h"
#include "Constants.h"

class AssetManager;
class EnemyDirector;
struct GameData;

class ProjectileManager : public IProjectileManager
{
public:
    ProjectileManager(AssetManager& _asset_manager, EnemyDirector& _enemy_director);
    ~ProjectileManager() = default;

    void tick(GameData& _gd);
    void draw(sf::RenderWindow& _window);

    void requestLaser(const sf::Vector2f& _from, const sf::Vector2f& _to) override;
    void requestBullet(const sf::Vector2f& _from, const sf::Vector2f& _to) override;

private:
    struct TowerLaser
    {
    public:
        TowerLaser();

        void draw(sf::RenderWindow& _window);

        void refresh(const sf::Vector2f& _from, const sf::Vector2f& _to);
        bool isVisible() const;

    private:
        sf::VertexArray line;
        sf::Color line_color;
        float visible_duration;
        float draw_until_time;
    };

    struct TowerBullet
    {
    public:
        TowerBullet();

        void setTexture(sf::Texture* _texture);
        void setColor(const sf::Color& _color);

        void tick();
        void draw(sf::RenderWindow& _window);

        void refresh(const sf::Vector2f& _from, const sf::Vector2f& _to);
        bool isVisible() const;

        void destroy();

        const sf::Vector2f& getPosition() const;

    private:
        TDSprite sprite;
        sf::Vector2f direction;

        float visible_duration;
        float draw_until_time;
    };

    void initBullets(AssetManager& _asset_manager);

    EnemyDirector& enemy_director;

    std::array<TowerLaser, MAX_TOWERS> lasers;
    std::array<TowerBullet, MAX_BULLETS> bullets;
    

};
