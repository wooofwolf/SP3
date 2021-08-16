#include "Guns.h"

#include <iostream>

GunComponent::~GunComponent()
{
}

void ConcreteGunComponent::FireGun() const
{
	std::cout << "Firing from ConcreteGunComponent" << std::endl;
}

GunDecorator::GunDecorator(GunComponent* component) : component(component)
{
}

void GunDecorator::FireGun() const
{
	std::cout << "Firing from GunDecorator" << std::endl;
}

Revolver::Revolver(GunComponent* component) : GunDecorator(component)
{
}

void Revolver::FireGun() const
{
	std::cout << "Firing from Revolver" << std::endl;
}

Shotgun::Shotgun(GunComponent* component) : GunDecorator(component)
{
}

void Shotgun::FireGun() const
{
	std::cout << "Firing from Shotgun" << std::endl;
}

Minigun::Minigun(GunComponent* component) : GunDecorator(component)
{
}

void Minigun::FireGun() const
{
	std::cout << "Firing from Minigun" << std::endl;
}
