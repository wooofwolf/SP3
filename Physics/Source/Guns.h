#pragma once

class GunComponent
{

public:
	GunComponent* nextGun;
	GunComponent* prevGun;

	virtual ~GunComponent();

	virtual void FireGun() const = 0;
};

class ConcreteGunComponent : public GunComponent
{
public:
	void FireGun() const override;
};

class GunDecorator : public GunComponent
{
protected:
	GunComponent* component;

public:
	GunDecorator(GunComponent* component);

	void FireGun() const override;
};

class Revolver : public GunDecorator
{
public:
	Revolver(GunComponent* component);

	void FireGun() const override;
};

class Shotgun : public GunDecorator
{
public:
	Shotgun(GunComponent* component);

	void FireGun() const override;
};

class Minigun : public GunDecorator
{
public:
	Minigun(GunComponent* component);

	void FireGun() const override;
};