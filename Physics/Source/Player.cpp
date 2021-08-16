#include "Player.h"
#include "Application.h"
#include "GL\glew.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Init()
{
	hp = 1;
	pos.Set(10, 90, 1);
	scale.Set(3, 3, 3);
	normal.Set(1, 0, 0);
	active = true;
}

void Player::Update(double dt)
{
	

	if (Application::IsKeyPressed('W'))
	{
		pos.y += 1;
	}	
	if (Application::IsKeyPressed('S'))
	{
		pos.y -= 1;
	}	
	if (Application::IsKeyPressed('A'))
	{
		pos.x -= 1;
	}	
	if (Application::IsKeyPressed('D'))
	{
		pos.x += 1;
	}
}

void Player::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	modelStack.LoadIdentity();

	modelStack.PushMatrix();
	{
		modelStack.Translate(pos.x, pos.y, pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2f(normal.y, normal.x)), 0, 0, 1);
		modelStack.Scale(scale.x, scale.y, scale.z);
		//RenderMesh(meshList[GEO_CUBE], true);
	}
	modelStack.PopMatrix();
}

void Player::Movement()
{

}

Vector3 Player::getPos()
{
	return pos;
}

Vector3 Player::getRotate()
{
	return normal;
}

Vector3 Player::getScale()
{
	return scale;
}