#pragma once

#include "Projectile.h"

class GunComponent
{

public:
	GunComponent* nextGun;
	GunComponent* prevGun;
	double fireRate;
	double fireRateTimer;

	virtual ~GunComponent();

	virtual Projectile* FireGun() = 0;
	void UpdateFireRateTimer(double dt);
};

class ConcreteGunComponent : public GunComponent
{
public:
	Projectile* FireGun() override;
};

class GunDecorator : public GunComponent
{
protected:
	GunComponent* component;

public:
	GunDecorator(GunComponent* component);

	Projectile* FireGun() override;
};

class Revolver : public GunDecorator
{
public:
	Revolver(GunComponent* component);

	Projectile* FireGun() override;
};

class Shotgun : public GunDecorator
{
public:
	Shotgun(GunComponent* component);

	Projectile* FireGun() override;
};

class Minigun : public GunDecorator
{
public:
	Minigun(GunComponent* component);

	Projectile* FireGun() override;
};