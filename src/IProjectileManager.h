#pragma once

struct ProjectileRequest;

class IProjectileManager
{
public:
    IProjectileManager() = default;
    ~IProjectileManager() = default;

    virtual void requestProjectile(const ProjectileRequest& _request) = 0;

};
