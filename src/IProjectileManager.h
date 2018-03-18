#pragma once

struct ProjectileRequest;

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Interface for the ProjectileManager to extend to expose limited
information to worker classes, such as Tower.

---------------------------------------------------------*/
class IProjectileManager
{
public:
    IProjectileManager() = default;
    ~IProjectileManager() = default;

    virtual void requestProjectile(const ProjectileRequest& _request) = 0;

};
