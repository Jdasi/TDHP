#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "EnemyListener.h"
#include "ListenerSubject.h"
#include "Killable.h"
#include "LevelPath.h"
#include "TDSprite.h"
#include "EnemyType.h"
#include "ValueBar.h"
#include "Scheduler.h"

class Enemy final : public TDSprite, public Killable,
    public ListenerSubject<EnemyListener>
{
public:
    Enemy();
    ~Enemy() = default;

    EnemyType* getType() const;
    void setType(EnemyType& _type);

    void tick() override;
    void draw(sf::RenderWindow& _window) override;

    void setPath(const LevelPath& _path);

    const sf::Vector2i& getCoords() const;
    int getDistToGoal() const;

    void boostHealth(const int _modifier, const float _duration);
    void boostSpeed(const float _modifier, const float _duration);

protected:
    // Killable events.
    void onSpawn() override;
    void onDamage(TowerType* _attacker_type) override;
    void onDeath(TowerType* _killer_type) override;

    // TDSprite events.
    void onSetPosition() override;

private:
    void initHealthBar();
    void initShieldShape();

    void updateTileIndex();
    void nextWaypoint();
    void moveToWaypoint(const Waypoint& _waypoint, const sf::Vector2f& _pos,
        float _remaining_dist_sqr);

    void resetHealth();
    void resetSpeed();

    LevelPath path;
    int path_index;

    EnemyType* type;

    ValueBar health_bar;
    Scheduler scheduler;

    sf::CircleShape shield_shape;
    bool shield_visible;

    float speed_modifier;
    sf::Vector2i coords;

    int mdist_to_goal;

};
