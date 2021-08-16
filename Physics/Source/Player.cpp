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
	pos.Set(1, -1, 0);
	scale.Set(1, 1, 1);
	normal.Set(1, 0, 0);
	active = true;
}

void Player::Update()
{
	if (Application::IsKeyPressed('W'))
	{

	}	
	if (Application::IsKeyPressed('S'))
	{

	}	
	if (Application::IsKeyPressed('A'))
	{

	}	
	if (Application::IsKeyPressed('D'))
	{

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