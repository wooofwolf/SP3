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

	m_speed = 1.f;
	SprintSpeed = 1.f;
}

void Player::Update(double dt)
{
	// Currently Hold Space to slow time, Change to a timer toggle later
	if (Application::IsKeyPressed(VK_SPACE))
	{
		m_speed = 0.5f;
	}
	else
	{
		m_speed = 1.f;
	}

	if (Application::IsKeyPressed('W'))
	{
		pos.y += 30 * dt * m_speed * SprintSpeed;
	}	
	if (Application::IsKeyPressed('S'))
	{
		pos.y -= 30 * dt * m_speed * SprintSpeed;
	}	
	if (Application::IsKeyPressed('A'))
	{
		pos.x -= 30 * dt * m_speed * SprintSpeed;
	}	
	if (Application::IsKeyPressed('D'))
	{
		pos.x += 30 * dt * m_speed * SprintSpeed;
	}
}

void Player::Render()
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//modelStack.LoadIdentity();

	//modelStack.PushMatrix();
	//{
	//	modelStack.Translate(pos.x, pos.y, pos.z);
	//	modelStack.Rotate(Math::RadianToDegree(atan2f(normal.y, normal.x)), 0, 0, 1);
	//	modelStack.Scale(scale.x, scale.y, scale.z);
	//	//RenderMesh(meshList[GEO_CUBE], true);
	//}
	//modelStack.PopMatrix();
}

void Player::Movement()
{

}

Vector3 Player::GetPos()
{
	return pos;
}

Vector3 Player::GetRotate()
{
	return normal;
}

Vector3 Player::GetScale()
{
	return scale;
}

float Player::GetGameSpeed()
{
	return m_speed;
}