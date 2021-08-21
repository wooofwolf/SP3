#include "Entity.h"

Entity::Entity()
	: hp(0.f),
	pos(Vector3()),
	vel(Vector3()),
	scale(Vector3()),
	normal(Vector3()),
	active(false)
{
}

Entity::~Entity()
{
}
