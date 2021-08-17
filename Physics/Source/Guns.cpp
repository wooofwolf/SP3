#include "Guns.h"

#include <iostream>

#include "Projectile.h"

GunComponent::~GunComponent()
{
}

void GunComponent::UpdateFireRateTimer(double dt)
{
	fireRateTimer += dt;
}

Projectile* ConcreteGunComponent::FireGun()
{
	std::cout << "Firing from ConcreteGunComponent" << std::endl;

	return new Projectile(Projectile::PROJECTILE_TYPE::PROJECTILE_NONE);
}

GunDecorator::GunDecorator(GunComponent* component) : component(component)
{
}

Projectile* GunDecorator::FireGun()
{
	std::cout << "Firing from GunDecorator" << std::endl;

	return new Projectile(Projectile::PROJECTILE_TYPE::PROJECTILE_NONE);
}

Revolver::Revolver(GunComponent* component) : GunDecorator(component)
{
}

Projectile* Revolver::FireGun()
{
	std::cout << "Firing from Revolver" << std::endl;

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
	std::cout << "Firing from Shotgun" << std::endl;

	if (fireRateTimer >= fireRate)
	{
		fireRateTimer = 0;
		return new Projectile(Projectile::PROJECTILE_TYPE::PROJECTILE_SHOTGUN);
	}
	return nullptr;
}

Minigun::Minigun(GunComponent* component) : GunDecorator(component)
{
}

Projectile* Minigun::FireGun()
{
	std::cout << "Firing from Minigun" << std::endl;

	if (fireRateTimer >= fireRate)
	{
		fireRateTimer = 0;
		return new Projectile(Projectile::PROJECTILE_TYPE::PROJECTILE_MINIGUN);
	}
	return nullptr;
}
