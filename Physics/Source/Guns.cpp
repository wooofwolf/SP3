#include "Guns.h"

#include <iostream>

#include "Projectile.h"
#include "Player.h"

GunComponent::~GunComponent()
{
}

void GunComponent::UpdateFireRateTimer(double dt)
{
	fireRateTimer += dt * Player::GetInstance()->GetPlusSpeed();
}

Projectile* ConcreteGunComponent::FireGun()
{
	return new Projectile(Projectile::PROJECTILE_TYPE::PROJECTILE_NONE);
}

GunDecorator::GunDecorator(GunComponent* component) : component(component)
{
}

Projectile* GunDecorator::FireGun()
{
	return new Projectile(Projectile::PROJECTILE_TYPE::PROJECTILE_NONE);
}

Revolver::Revolver(GunComponent* component) : GunDecorator(component)
{
}

Projectile* Revolver::FireGun()
{
	if (fireRateTimer >= fireRate)
	{
		fireRateTimer = 0;
		return new Projectile(Projectile::PROJECTILE_TYPE::PROJECTILE_REVOLVER);
	}
	return nullptr;
}

Shotgun::Shotgun(GunComponent* component) : GunDecorator(component)
{
}

Projectile* Shotgun::FireGun()
{
	return new Projectile(Projectile::PROJECTILE_TYPE::PROJECTILE_SHOTGUN);
}

Minigun::Minigun(GunComponent* component) : GunDecorator(component)
{
}

Projectile* Minigun::FireGun()
{
	if (fireRateTimer >= fireRate)
	{
		fireRateTimer = 0;
		return new Projectile(Projectile::PROJECTILE_TYPE::PROJECTILE_MINIGUN);
	}
	return nullptr;
}
